Raw Linux profiles (`*.profraw.zst`) recorded with the instrumented build
(`release-binaries.yml`, `pgo_mode: generate`) playing Ocarina of Time.
The Linux PGO build merges them into `oot-linux.profdata` and uses that in place
of `eglantine.profdata`. Without files here nothing changes.
