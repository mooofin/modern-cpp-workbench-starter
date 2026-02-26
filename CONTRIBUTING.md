# Contributing

Thanks for hacking on this repo. Keep it simple and fast.

## Setup
- Install toolchain (clang, cmake, ninja/make) and optional helpers: clang-tidy, clang-format, cppcheck, include-what-you-use, llvm-cov/profdata, perf.
- Install Conan 2.x.
- (Optional) `pre-commit install` to get format/tidy on commit.

## Common flows
- Native dev: `./dev.sh` (Debug, sanitizers, clang-tidy on).
- Conan: `conan install . --output-folder=build-conan --build=missing -s build_type=Debug` then `PRESET=conan-debug ./dev.sh`.
- Profiling: `PROFILING=ON SAN=OFF ./dev.sh` or `cmake --preset profile && cmake --build --preset profile` then `perf record --call-graph=dwarf ./build-profile/TemplatePlayground`.

## Style
- Formatting: clang-format (config in `.clang-format`), cmake-format for CMake.
- Static analysis: clang-tidy default ON; run `cmake --build <dir>` to execute. `cppcheck` target if installed. IWYU optional.
- Tests: `ctest` via `dev.sh`. In sandbox use `ASAN_OPTIONS=detect_leaks=0` to dodge LSan ptrace issues.

## PR checklist
- Code builds (`./dev.sh` or preset of choice).
- Tests pass.
- Formatting/clang-tidy clean (`pre-commit run --all-files` if installed).
- Update docs if flags or workflow changed.

## Releases
Not automated yet; tag + changelog entry is enough.
