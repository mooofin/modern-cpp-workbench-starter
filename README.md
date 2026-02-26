# C++ Starter (CMake + Conan)

Modern C++20 starter with sane defaults: warnings-on, sanitizers, clang-tidy, optional IWYU/cppcheck, and Conan deps.

## Quick start
- Native build (Debug + sanitizers + clang-tidy):
  ```bash
  ./dev.sh
  ```
- Conan toolchain build (uses `conan-debug` preset created by `conan install`):
  ```bash
  conan install . --output-folder=build-conan --build=missing -s build_type=Debug
  PRESET=conan-debug ./dev.sh
  ```
- LeakSanitizer workaround here: `ASAN_OPTIONS=detect_leaks=0` (default via `LEAKS=0`). Set `LEAKS=1` to enable.

## Options & tooling
See `docs/tooling.md` for all switches. Highlights:
- Sanitizers: `-DENABLE_SANITIZERS=ON` (Debug) with `-DSANITIZERS_LIST="address;undefined;thread"`.
- Coverage: `-DENABLE_COVERAGE=ON` (Clang) then `cmake --build <dir> --target coverage`.
- Profiling: `-DENABLE_PROFILING=ON` or `PROFILING=ON` in `dev.sh`; see `docs/profiling.md`.
- Static analysis: clang-tidy (default ON), cppcheck target, optional include-what-you-use (`-DENABLE_IWYU=ON`).
- Pre-commit hooks: `.pre-commit-config.yaml` runs clang-format, cmake-format, optional clang-tidy.

## Presets & profiles
- Native: `build` directory by default.
- Conan: `conan install` generates `build-conan` and preset `conan-debug`.
- Sample Conan profiles: `conan-profiles/clang`, `conan-profiles/gcc` (`conan install . -pr conan-profiles/clang`).

## Targets
- `TemplatePlayground` — demo binary showcasing modern non-math C++20 patterns:
  concepts, `std::variant` visitation, ranges views, spans, and optional/reference-wrapper lookups.
- `container_tests` — GTest suite (labels: `unit;sanitizers`).
- `cppcheck` — static analysis (if tool installed).
- `coverage` — HTML coverage via LLVM tools when enabled.

## Scripts
- `dev.sh` — configure/build/test wrapper. Env vars: `BUILD_DIR`, `BUILD_TYPE`, `CLANG_TIDY`, `SAN`, `SAN_LIST`, `COVERAGE`, `LEAKS`, `PRESET`, `IWYU`, `PROFILING`.

More notes in `docs/runtime.md`, `docs/tooling.md`, and `docs/profiling.md`.
