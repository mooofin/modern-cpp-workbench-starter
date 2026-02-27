# Overview

This project is a practical C++20 starter, not a toy skeleton.

You get:
- A modern CMake setup with strong warnings and optional LTO
- Unit tests (GTest via CMake/Conan, standalone test target for Bazel)
- Sanitizers and optional coverage/profiling flags
- Optional static analysis hooks (clang-tidy, cppcheck, IWYU)
- A one-command local loop in `./dev.sh`

Main files:
- `main.cpp` demo app target: `TemplatePlayground`
- `include/container.hpp` small reusable container utility
- `tests/container_test.cpp` CMake/Conan GTest suite
- `tests/container_bazel_test.cpp` Bazel-friendly test binary

If you are starting a new project from this repo, first rename the binary/package names and replace the demo logic in `main.cpp`.
