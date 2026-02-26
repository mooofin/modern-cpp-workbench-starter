# modern-cpp-workbench-starter

A practical C++20 starter repo with build tooling, tests, and safety checks already wired in.

## What this gives you

- CMake project setup for C++20
- Conan integration for dependencies
- Fast local dev script (`dev.sh`)
- Sanitizers for memory and UB checks
- Clang-Tidy, optional cppcheck, optional IWYU
- GTest test target
- Profiling helper script (`scripts/profile.sh`)

## Project layout

- `main.cpp`: demo app (`TemplatePlayground` target)
- `include/container.hpp`: sample generic container utilities
- `tests/container_test.cpp`: unit tests
- `docs/`: short notes for runtime, tooling, profiling
- `dev.sh`: configure, build, and test in one command

## Quick start

### 1. Native build

```bash
./dev.sh
```

This runs configure + build + tests in Debug mode with useful defaults.

### 2. Conan build

```bash
conan install . --output-folder=build-conan --build=missing -s build_type=Debug
PRESET=conan-debug ./dev.sh
```

## Useful commands

### Build only

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
```

### Run tests

```bash
ctest --test-dir build --output-on-failure
```

If LeakSanitizer causes issues in sandboxed environments:

```bash
ASAN_OPTIONS=detect_leaks=0 ctest --test-dir build --output-on-failure
```

### Run the demo binary

```bash
./build/TemplatePlayground
```

## CMake options you will likely use

- `-DENABLE_SANITIZERS=ON`
- `-DSANITIZERS_LIST="address;undefined"`
- `-DENABLE_CLANG_TIDY=ON`
- `-DENABLE_CPPCHECK=ON`
- `-DENABLE_IWYU=OFF`
- `-DENABLE_COVERAGE=OFF`
- `-DENABLE_PROFILING=OFF`

## Profiling

```bash
PROFILING=ON SAN=OFF ./dev.sh
ASAN_OPTIONS=detect_leaks=0 ./scripts/profile.sh
```

## Notes

- This repo currently uses `master` as the default branch.
- If you use this as a base for a new project, start by renaming the target and package metadata to your app name.
