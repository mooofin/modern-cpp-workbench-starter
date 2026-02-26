#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR=${BUILD_DIR:-build}
PRESET=${PRESET:-}
BUILD_TYPE=${BUILD_TYPE:-Debug}
CLANG_TIDY=${CLANG_TIDY:-ON}
SAN=${SAN:-ON}
SAN_LIST=${SAN_LIST:-address;undefined}
COVERAGE=${COVERAGE:-OFF}
LEAKS=${LEAKS:-0}
IWYU=${IWYU:-OFF}
PROFILING=${PROFILING:-OFF}

cmake_common_args=(
  -DENABLE_CLANG_TIDY=${CLANG_TIDY}
  -DENABLE_SANITIZERS=${SAN}
  -DSANITIZERS_LIST=${SAN_LIST}
  -DENABLE_COVERAGE=${COVERAGE}
  -DENABLE_IWYU=${IWYU}
  -DENABLE_PROFILING=${PROFILING}
)

if [[ -n "${PRESET}" ]]; then
  cmake --preset "${PRESET}" "${cmake_common_args[@]}"
  cmake --build --preset "${PRESET}"
  ASAN_OPTIONS=detect_leaks=${LEAKS} ctest --preset "${PRESET}" --output-on-failure
else
  cmake -S . -B "${BUILD_DIR}" -DCMAKE_BUILD_TYPE=${BUILD_TYPE} "${cmake_common_args[@]}"
  cmake --build "${BUILD_DIR}"
  ASAN_OPTIONS=detect_leaks=${LEAKS} ctest --test-dir "${BUILD_DIR}" --output-on-failure
fi
