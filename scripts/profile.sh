#!/usr/bin/env bash
set -euo pipefail

PRESET=${PRESET:-profile}
BINARY=${BINARY:-TemplatePlayground}
LEAKS=${LEAKS:-0}

cmake --preset "${PRESET}"
cmake --build --preset "${PRESET}"

# Try to locate the built binary; fallback to expected path
BIN_DIR=$(cmake --preset "${PRESET}" --show-logs 2>/dev/null | grep -o "Build files have been written to: .*" | awk '{print $8}')
BIN_DIR=${BIN_DIR:-"build-profile"}
BIN_PATH=${BIN_DIR}/${BINARY}

ASAN_OPTIONS=detect_leaks=${LEAKS} perf record --call-graph=dwarf "${BIN_PATH}"
perf report
