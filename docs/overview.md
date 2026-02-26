# Project overview

This repo is a small C++20 starter meant to feel good out of the box:
- Clean builds: warnings-as-errors, modern CMake, LTO toggle.
- Safety nets: ASan/UBSan (configurable list), optional coverage, profiler-friendly preset.
- Tooling: clang-tidy on by default, cppcheck target, IWYU optional, clang-format/cmake-format hooks.
- Tests: GTest suite with sensible labels and Conan integration.
- Helpers: `dev.sh` for quick loops, Conan profiles, profiling script.

Use it as a seed for experiments or as a template for new projects.

Current demo executable: `TemplatePlayground` in `main.cpp`, built around an event/action pipeline (not numeric/math examples) using modern C++20 features.
