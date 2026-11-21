> [!WARNING]
> **Test build.** This release is for testing on the Steam Deck and the Legion Go S and will be replaced by a regular
> release once it is confirmed.

> [!NOTE]
> **From v1.0.6 this update arrives by itself:** LaküenPlay says at start that v1.0.11 is out, downloads it, replaces
> itself and starts again. Coming from v1.0.5 or older, download this one by hand once.

**LaküenPlay v1.0.11 – Handheld** brings Lichii's work of 20 and 21 November: stability and memory fixes in
the renderer, and the shader builds left shown in the status bar.

> [!IMPORTANT]
> This is an unofficial fork, not an official Lichii release. Please report problems [here](https://github.com/elkarbeenus/LakuenPlay/issues), not upstream. Games, keys and firmware are not included: only use content you have legally obtained.

## Downloads

| File | What it is |
| --- | --- |
| `LakuenPlay-v1.0.11-Handheld-x86_64-pgo.AppImage` | **Recommended.** Self-contained, compiled for Zen 2 and later with profile-guided optimization (a profile recorded on a Steam Deck) |
| `LakuenPlay-v1.0.11-Handheld-x86_64.AppImage` | The same program without PGO |
| `*.tar.gz` | Plain binaries that use your system libraries |
| `SHA256SUMS.txt` | Checksums |

## From Lichii (20-21 November)

- **Stability:** texel buffer views are clamped to the device's limit and a failed one no longer takes the renderer down
  ([#2296](https://github.com/zortos293/Lichii/pull/2296)); sparse segments that no longer translate are skipped
  ([#2360](https://github.com/zortos293/Lichii/pull/2360)); a compute pipeline dropped before it ran counts as done
  ([#1799](https://github.com/zortos293/Lichii/pull/1799)); a log another instance still writes is not moved
  ([#2256](https://github.com/zortos293/Lichii/pull/2256)); the config file is synced before it is renamed
  ([#1972](https://github.com/zortos293/Lichii/pull/1972)).
- **Memory:** smaller device-buffer blocks on GPUs with little VRAM ([#2316](https://github.com/zortos293/Lichii/pull/2316)); the
  texture cache's spill collection keeps the working set instead of deleting every image unused for 10 frames.
- **Shaders:** the status bar shows how many shader builds are left ([#1800](https://github.com/zortos293/Lichii/pull/1800));
  driver blobs stored in the same session are reused ([#2349](https://github.com/zortos293/Lichii/pull/2349)); the cloth step
  follows the frame rate below 60 fps ([#2313](https://github.com/zortos293/Lichii/pull/2313)).
- Also: Switch 2 Edition boot fixes, online-play fixes, and fixes for other games.

This fork's handheld work (memory guard, low-RAM texture limit, heap trimming, pipeline eviction, the depth clear without
an upload, the recommended handheld settings, the master log, updates from this repository) stays as in v1.0.10.

## Credits

Built on **[Lichii](https://github.com/zortos293/Lichii)** (zortos293 and contributors), **[Eglantine](https://github.com/Djihads80/Eglantine)**, **Eden**, **yuzu**, **Sudachi** and **Citra**, with work from [Fritchii](https://github.com/Frizti/Fritchii). Thank you!
