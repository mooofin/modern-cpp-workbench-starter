#!/bin/bash

export ASAN_OPTIONS=detect_leaks=1
export ASAN_OPTIONS+=:detect_stack_use_after_return=1
export ASAN_OPTIONS+=:strict_string_checks=1
export ASAN_OPTIONS+=:halt_on_error=1
export ASAN_OPTIONS+=:verbosity=2
export ASAN_OPTIONS+=:symbolize=1

export UBSAN_OPTIONS=print_stacktrace=1
export UBSAN_OPTIONS+=:halt_on_error=1

cd "$(dirname "$0")" || exit 1

cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/TemplatePlayground
