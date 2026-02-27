# C++ Starter Kit

Small C++20 starter with CMake, Conan, tests, and sanitizers.

## Quick Start

```bash
./dev.sh
```

## Common Commands

```bash
# build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j

# test
ctest --test-dir build --output-on-failure

# run
./build/TemplatePlayground
```

## Conan

```bash
conan install . --output-folder=build-conan --build=missing -s build_type=Debug
PRESET=conan-debug ./dev.sh
```

## Bazel

```bash
# build
bazel --batch --output_user_root=/tmp/bazel build //:TemplatePlayground

# run
bazel --batch --output_user_root=/tmp/bazel run //:TemplatePlayground

# test
bazel --batch --output_user_root=/tmp/bazel test //:container_test
```

Offline config is in `.bazelrc` + `WORKSPACE`.
