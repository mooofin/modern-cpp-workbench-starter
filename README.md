# C++ Starter Kit

This is a practical C++20 starter you can clone and use right away. It includes CMake, Conan, Bazel, tests, and safety tooling so you can start coding instead of wiring build files all day.

## Install Dependencies

### Arch Linux

```bash
sudo pacman -S --needed base-devel cmake ninja gcc clang llvm lld gdb lldb gtest cppcheck include-what-you-use pkgconf python python-pipx bazelisk git
pipx ensurepath
pipx install conan
```

### NixOS

Use the included shell:

```bash
nix-shell
```

Or add the Home Manager module from this repo:

```nix
# home.nix
{
  imports = [
    /path/to/cpp-starter-kit/nix/home-manager.nix
  ];
}
```

### Debian / Ubuntu

```bash
sudo apt update
sudo apt install -y build-essential cmake ninja-build gcc clang lld llvm gdb lldb libgtest-dev cppcheck iwyu pkg-config python3 python3-pip pipx git
pipx ensurepath
pipx install conan
```

### Void Linux

```bash
sudo xbps-install -Su
sudo xbps-install -y base-devel cmake ninja gcc clang llvm lld gdb lldb gtest-devel cppcheck include-what-you-use pkg-config python3 python3-pip git
python3 -m pip install --user --upgrade pip
python3 -m pip install --user conan
```

### Gentoo

```bash
sudo emerge --ask \
  sys-devel/gcc llvm-core/clang llvm-core/lld llvm-core/llvm \
  dev-build/cmake dev-build/ninja dev-debug/gdb dev-util/lldb \
  dev-cpp/gtest dev-util/cppcheck dev-util/include-what-you-use \
  dev-util/pkgconf dev-python/pip dev-python/pipx dev-vcs/git
pipx ensurepath
pipx install conan
```

## Start Here

If you want the fastest path, run `./dev.sh`. It configures, builds, and tests in one pass, so you can quickly confirm your setup works.

## Pick a Build Flow

### CMake (native)

Use this if you prefer working directly with CMake. Configure with `cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug`, build with `cmake --build build --parallel`, run tests with `ctest --test-dir build --output-on-failure`, and launch the demo with `./build/TemplatePlayground`.

### LLVM dev setup (Clang + optional lld/libc++)

If you want a Clang-first workflow, run `TOOLCHAIN=llvm USE_LLD=ON ./dev.sh`. If you also want libc++, use `TOOLCHAIN=llvm USE_LLD=ON USE_LIBCXX=ON ./dev.sh`. Prefer presets? Use `cmake --preset llvm-debug`, `cmake --build --preset llvm-debug --parallel`, and `ctest --preset llvm-debug --output-on-failure`.

### Conan (dependency-managed CMake)

If you want Conan to manage dependencies, run `conan install . --output-folder=build-conan --build=missing -s build_type=Debug`, then run `PRESET=conan-debug ./dev.sh`.

### Bazel (offline-friendly config included)

If you prefer Bazel, run `bazel --batch --output_user_root=/tmp/bazel build //:TemplatePlayground`, then `bazel --batch --output_user_root=/tmp/bazel test //:container_test`, and finally `bazel --batch --output_user_root=/tmp/bazel run //:TemplatePlayground`. This repo includes `WORKSPACE` and `.bazelrc` set up for offline-friendly usage.

## Useful `dev.sh` switches

`dev.sh` is controlled with environment variables. A common example is `JOBS=8 SAN=ON SAN_LIST='address;undefined' CLANG_TIDY=ON ./dev.sh`. The switches you’ll use most are `JOBS`, `TOOLCHAIN` (`default`, `llvm`, `clang`, or `gcc`), `SAN`, `SAN_LIST`, `CLANG_TIDY`, `COVERAGE`, `IWYU`, `PROFILING`, `USE_LLD`, `USE_LIBCXX` (Clang only), `LEAKS`, and `PRESET`.

## Where to read next

If you want more detail, check `docs/overview.md` for the project map, `docs/tooling.md` for build and analysis switches, `docs/runtime.md` for sanitizer and coverage behavior, and `docs/profiling.md` for profiling notes.
