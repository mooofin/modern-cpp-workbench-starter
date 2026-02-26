# Profiling quickstart

1) Build with profiling flags (frame pointers on, extra debug info, -O3 for non-Debug):
   ```bash
   PROFILING=ON SAN=OFF ./dev.sh
   # or with Conan preset
   PROFILING=ON SAN=OFF PRESET=conan-debug ./dev.sh
   # or use the dedicated preset
   cmake --preset profile && cmake --build --preset profile
   ```
2) Record with Linux `perf` (DWARF call stacks):
   ```bash
   ASAN_OPTIONS=detect_leaks=0 perf record --call-graph=dwarf ./build-profile/TemplatePlayground
   perf report
   ```
   Or use the helper: `./scripts/profile.sh` (vars: `PRESET`, `BINARY`, `LEAKS`).
3) GUI viewers: Hotspot or `perf report --stdio` as above.

Notes
- Avoid sanitizers while profiling (`SAN=OFF`) to keep overhead low.
- Coverage and profiling can be combined, but results may be noisy; prefer profiling-only builds.
- TSan/ASan add significant overhead; use only if profiling threading or memory bugs specifically.
