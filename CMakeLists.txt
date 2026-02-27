cmake_minimum_required(VERSION 3.20)
project(TemplatePlaygroundCpp LANGUAGES CXX VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Pick a sensible default build type for single-config generators.
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

option(ENABLE_WARNINGS_AS_ERRORS "Treat warnings as errors" ON)
option(ENABLE_LTO "Enable IPO/LTO if supported" ON)
option(ENABLE_CLANG_TIDY "Run clang-tidy during builds" ON)
option(ENABLE_CPPCHECK "Add cppcheck static analysis target" ON)
option(ENABLE_IWYU "Add include-what-you-use analysis during builds" OFF)
option(ENABLE_COVERAGE "Enable LLVM source-based coverage (Clang only)" OFF)
option(ENABLE_SANITIZERS "Enable sanitizers (Debug only)" ON)
option(ENABLE_PROFILING "Add frame pointers/flags suitable for perf" OFF)
option(ENABLE_LLD "Use lld linker when available" OFF)
option(USE_LIBCXX "Use libc++ with Clang" OFF)
set(SANITIZERS_LIST "address;undefined" CACHE STRING "Semicolon-separated sanitizers (address;undefined;thread;leak)")

add_library(project_options INTERFACE)
set(PROJECT_WARNING_FLAGS
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wconversion)
if(ENABLE_WARNINGS_AS_ERRORS)
  list(APPEND PROJECT_WARNING_FLAGS -Werror)
endif()
target_compile_options(project_options INTERFACE ${PROJECT_WARNING_FLAGS})
target_compile_features(project_options INTERFACE cxx_std_20)

target_compile_options(
  project_options
  INTERFACE
    $<$<CONFIG:Debug>:-O1;-g3>
    $<$<NOT:$<CONFIG:Debug>>:-O2>)

if(USE_LIBCXX)
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(project_options INTERFACE -stdlib=libc++)
    target_link_options(project_options INTERFACE -stdlib=libc++)
  else()
    message(WARNING "USE_LIBCXX=ON ignored because compiler is ${CMAKE_CXX_COMPILER_ID}")
  endif()
endif()

if(ENABLE_LLD)
  find_program(LLD_LINKER NAMES ld.lld lld-link)
  if(LLD_LINKER)
    target_link_options(project_options INTERFACE -fuse-ld=lld)
  else()
    message(WARNING "ENABLE_LLD=ON but lld linker not found")
  endif()
endif()

if(ENABLE_SANITIZERS)
  string(REPLACE ";" "," SANITIZER_LIST "${SANITIZERS_LIST}")
  target_compile_options(
    project_options
    INTERFACE
      $<$<CONFIG:Debug>:-fsanitize=${SANITIZER_LIST};-fno-omit-frame-pointer;-g>)
  target_link_options(project_options INTERFACE $<$<CONFIG:Debug>:-fsanitize=${SANITIZER_LIST}>)
endif()

if(ENABLE_PROFILING)
  if(ENABLE_SANITIZERS)
    message(WARNING "Profiling with sanitizers enabled may skew results; consider -DENABLE_SANITIZERS=OFF")
  endif()
  target_compile_options(project_options INTERFACE -fno-omit-frame-pointer -g)
  target_link_options(project_options INTERFACE -fno-omit-frame-pointer)
  target_compile_options(project_options INTERFACE $<$<NOT:$<CONFIG:Debug>>:-O3>)
endif()

if(ENABLE_LTO)
  include(CheckIPOSupported)
  check_ipo_supported(RESULT ipo_supported)
  if(ipo_supported)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
  endif()
endif()

if(ENABLE_CLANG_TIDY)
  find_program(CLANG_TIDY_EXE NAMES clang-tidy)
  if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_EXE})
  else()
    message(WARNING "ENABLE_CLANG_TIDY=ON but clang-tidy not found")
  endif()
endif()

if(ENABLE_IWYU)
  find_program(IWYU_EXE NAMES iwyu include-what-you-use)
  if(IWYU_EXE)
    set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${IWYU_EXE})
  else()
    message(WARNING "ENABLE_IWYU=ON but include-what-you-use not found")
  endif()
endif()

if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  target_compile_options(project_options INTERFACE -fprofile-instr-generate -fcoverage-mapping)
  target_link_options(project_options INTERFACE -fprofile-instr-generate -fcoverage-mapping)
elseif(ENABLE_COVERAGE)
  message(WARNING "Coverage currently configured for Clang; requested but compiler is ${CMAKE_CXX_COMPILER_ID}")
endif()

add_library(container_lib INTERFACE)
target_include_directories(container_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(container_lib INTERFACE project_options)

add_executable(TemplatePlayground main.cpp)
target_link_libraries(TemplatePlayground PRIVATE container_lib)

include(CTest)
enable_testing()

if(BUILD_TESTING)
  find_package(GTest CONFIG REQUIRED)
  add_executable(container_tests tests/container_test.cpp)
  target_link_libraries(container_tests PRIVATE container_lib GTest::gtest_main)
  add_test(NAME container_tests COMMAND container_tests)
  set_tests_properties(container_tests PROPERTIES LABELS "unit;sanitizers")
else()
  message(STATUS "BUILD_TESTING=OFF, skipping unit test targets")
endif()

if(ENABLE_CPPCHECK)
  find_program(CPPCHECK_EXE NAMES cppcheck)
  if(CPPCHECK_EXE)
    add_custom_target(
      cppcheck
      COMMAND
        ${CPPCHECK_EXE}
          --std=c++20
          --enable=warning,performance,style
          --inline-suppr
          --quiet
          -i ${CMAKE_BINARY_DIR}
          ${CMAKE_CURRENT_SOURCE_DIR}/include
          ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
          ${CMAKE_CURRENT_SOURCE_DIR}/tests
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      COMMENT "Running cppcheck")
  else()
    message(WARNING "ENABLE_CPPCHECK=ON but cppcheck not found")
  endif()
endif()

if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND BUILD_TESTING)
  find_program(LLVM_PROFDATA_EXE NAMES llvm-profdata)
  find_program(LLVM_COV_EXE NAMES llvm-cov)
  if(LLVM_PROFDATA_EXE AND LLVM_COV_EXE)
    add_custom_target(
      coverage
      COMMAND ${CMAKE_COMMAND} -E make_directory coverage
      COMMAND ${CMAKE_COMMAND} -E env LLVM_PROFILE_FILE=coverage/%p.profraw
              ctest --output-on-failure
      COMMAND ${LLVM_PROFDATA_EXE} merge -sparse coverage/*.profraw -o coverage/coverage.profdata
      COMMAND ${LLVM_COV_EXE} show $<TARGET_FILE:container_tests> $<TARGET_FILE:TemplatePlayground>
              -instr-profile=coverage/coverage.profdata
              -format=html
              -output-dir=coverage/html
              -show-region-summary
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
      DEPENDS TemplatePlayground container_tests
      COMMENT "Running coverage with llvm-cov")
  else()
    message(WARNING "Coverage requested but llvm-profdata/llvm-cov not found")
  endif()
elseif(ENABLE_COVERAGE AND NOT BUILD_TESTING)
  message(WARNING "Coverage requested but BUILD_TESTING=OFF; coverage target not created")
endif()
