{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {
  packages = with pkgs; [
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

  shellHook = ''
    export CC=clang
    export CXX=clang++
    echo "cpp-starter-kit dev shell ready"
  '';
}
