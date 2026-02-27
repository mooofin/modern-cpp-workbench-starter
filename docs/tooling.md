# Tooling Switches

Most build behavior is controlled by CMake options.

## Core switches

- `ENABLE_CLANG_TIDY` (default `ON`): run clang-tidy during CMake builds
- `ENABLE_CPPCHECK` (default `ON`): adds `cppcheck` target when cppcheck exists
- `ENABLE_IWYU` (default `OFF`): include-what-you-use integration
- `ENABLE_WARNINGS_AS_ERRORS` (default `ON`): treat warnings as errors
- `ENABLE_LTO` (default `ON`): IPO/LTO when supported
- `ENABLE_LLD` (default `OFF`): use `lld` linker when available

## Runtime-quality switches

- `ENABLE_SANITIZERS` (default `ON`): enable sanitizer flags in Debug
- `SANITIZERS_LIST` (default `address;undefined`): semicolon-separated list
- `ENABLE_COVERAGE` (default `OFF`): LLVM coverage flow (Clang)
- `ENABLE_PROFILING` (default `OFF`): profiling-friendly flags
- `USE_LIBCXX` (default `OFF`): use libc++ with Clang

## `dev.sh` shortcuts

`dev.sh` maps env vars to these options, so you can do:

```bash
TOOLCHAIN=llvm USE_LLD=ON CLANG_TIDY=OFF SAN=OFF PROFILING=ON ./dev.sh
```

Useful vars:
- `JOBS`, `BUILD_DIR`, `BUILD_TYPE`, `PRESET`, `TOOLCHAIN`
- `CLANG_TIDY`, `SAN`, `SAN_LIST`, `COVERAGE`, `IWYU`, `PROFILING`, `LEAKS`, `USE_LLD`, `USE_LIBCXX`

Toolchain values:
- `TOOLCHAIN=default` uses your system default compiler
- `TOOLCHAIN=llvm` or `TOOLCHAIN=clang` uses `clang/clang++`
- `TOOLCHAIN=gcc` uses `gcc/g++`
