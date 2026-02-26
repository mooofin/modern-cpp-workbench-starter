# Tooling Switches

- `ENABLE_CLANG_TIDY` (default ON): set `-DENABLE_CLANG_TIDY=OFF` to skip; uses `.clang-tidy` profile.
- `ENABLE_CPPCHECK` (default ON): adds `cppcheck` target if tool is present.
- `ENABLE_IWYU` (default OFF): sets `CMAKE_CXX_INCLUDE_WHAT_YOU_USE` when include-what-you-use is available.
- `ENABLE_COVERAGE` (default OFF): Clang-only; enables LLVM coverage flags and `coverage` target.
- `ENABLE_SANITIZERS` (default ON Debug): controlled by `SANITIZERS_LIST` (semicolon-separated, e.g., `address;undefined;thread`).
- `ENABLE_LTO` (default ON): toggles IPO when supported.
- `ENABLE_WARNINGS_AS_ERRORS` (default ON): self-explanatory.

Helper script: `dev.sh` honors env vars `CLANG_TIDY`, `SAN`, `SAN_LIST`, `COVERAGE`, `LEAKS`, `PRESET`, `BUILD_TYPE`, `BUILD_DIR`, `IWYU`.

Recommended tools to install locally: clang-format, clang-tidy, cppcheck, include-what-you-use, cmake-format, llvm-profdata/llvm-cov (for coverage).
