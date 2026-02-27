#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR=${BUILD_DIR:-build}
PRESET=${PRESET:-}
BUILD_TYPE=${BUILD_TYPE:-Debug}
TOOLCHAIN=${TOOLCHAIN:-default}
JOBS=${JOBS:-$(nproc 2>/dev/null || echo 4)}
CLANG_TIDY=${CLANG_TIDY:-ON}
SAN=${SAN:-ON}
SAN_LIST=${SAN_LIST:-address;undefined}
COVERAGE=${COVERAGE:-OFF}
LEAKS=${LEAKS:-0}
IWYU=${IWYU:-OFF}
PROFILING=${PROFILING:-OFF}
USE_LIBCXX=${USE_LIBCXX:-OFF}
USE_LLD=${USE_LLD:-OFF}

cmake_common_args=(
  -DENABLE_CLANG_TIDY=${CLANG_TIDY}
  -DENABLE_SANITIZERS=${SAN}
  -DSANITIZERS_LIST=${SAN_LIST}
  -DENABLE_COVERAGE=${COVERAGE}
  -DENABLE_IWYU=${IWYU}
  -DENABLE_PROFILING=${PROFILING}
  -DUSE_LIBCXX=${USE_LIBCXX}
  -DENABLE_LLD=${USE_LLD}
)

toolchain_args=()
case "${TOOLCHAIN}" in
  default) ;;
  llvm|clang)
    toolchain_args=(-DCMAKE_CXX_COMPILER=clang++)
    ;;
  gcc)
    toolchain_args=(-DCMAKE_CXX_COMPILER=g++)
    ;;
  *)
    echo "Unknown TOOLCHAIN='${TOOLCHAIN}'. Use one of: default, llvm, clang, gcc" >&2
    exit 1
    ;;
esac

echo "==> Configure"
if [[ -n "${PRESET}" ]]; then
  if [[ "${TOOLCHAIN}" != "default" ]]; then
    echo "==> Note: TOOLCHAIN is ignored when PRESET is set. Use a preset-specific compiler."
  fi
  cmake --preset "${PRESET}" "${cmake_common_args[@]}"
  echo "==> Build"
  cmake --build --preset "${PRESET}" --parallel "${JOBS}"
  echo "==> Test"
  ASAN_OPTIONS=detect_leaks=${LEAKS} ctest --preset "${PRESET}" --output-on-failure
else
  cmake -S . -B "${BUILD_DIR}" -DCMAKE_BUILD_TYPE=${BUILD_TYPE} "${toolchain_args[@]}" "${cmake_common_args[@]}"
  echo "==> Build"
  cmake --build "${BUILD_DIR}" --parallel "${JOBS}"
  echo "==> Test"
  ASAN_OPTIONS=detect_leaks=${LEAKS} ctest --test-dir "${BUILD_DIR}" --output-on-failure
fi

echo "==> Done"
