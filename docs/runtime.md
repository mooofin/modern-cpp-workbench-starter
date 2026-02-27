# Runtime Notes

## Sanitizers

Debug builds use sanitizers by default (`address;undefined`).

Examples:

```bash
# default sanitizer setup
./dev.sh

# custom sanitizer list
SAN=ON SAN_LIST='address;undefined' ./dev.sh

# disable sanitizers
SAN=OFF ./dev.sh
```

Avoid combining ThreadSanitizer with AddressSanitizer in one run.

## LeakSanitizer

If your environment blocks leak checks (common in restricted sandboxes), run with:

```bash
LEAKS=0 ./dev.sh
```

That sets `ASAN_OPTIONS=detect_leaks=0` for test execution.

## Coverage

Coverage is Clang-oriented in this repo.

```bash
cmake -S . -B build-cov -DENABLE_COVERAGE=ON
cmake --build build-cov --target coverage
```

Report output: `build-cov/coverage/html/`
