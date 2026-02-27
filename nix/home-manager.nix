{ pkgs, ... }:
{
  home.packages = with pkgs; [
    cmake
    ninja
    gcc
    clang
    lld
    llvm
    gdb
    lldb
    gtest
    cppcheck
    include-what-you-use
    pkg-config
    python3
    python3Packages.pip
    # conan
    bazelisk
    git
  ];
}
