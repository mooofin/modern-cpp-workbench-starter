# Profiling Quickstart

Use a profiling-oriented build with sanitizers off.

```bash
PROFILING=ON SAN=OFF ./dev.sh
```

Or use preset flow:

```bash
cmake --preset profile
cmake --build --preset profile
```

## Record with `perf`

```bash
ASAN_OPTIONS=detect_leaks=0 perf record --call-graph=dwarf ./build-profile/TemplatePlayground
perf report
```

You can also use:

```bash
./scripts/profile.sh
```

## Practical tips

- Turn sanitizers off while profiling unless you are profiling sanitizer overhead.
- Keep input/workload consistent between runs.
- Compare profiles before and after changes, not isolated single runs.
