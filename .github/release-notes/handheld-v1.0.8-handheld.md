> [!WARNING]
> **Test build.** This release is for testing on the Steam Deck and the Legion Go S and will be replaced by a regular
> release once it is confirmed.

> [!NOTE]
> **From v1.0.6 this update arrives by itself:** LaküenPlay says at start that v1.0.8 is out, downloads it, replaces
> itself and starts again. Coming from v1.0.5 or older, download this one by hand once.

**LaküenPlay v1.0.8 – Handheld** brings Lichii's work of 16 and 17 November: fewer crashes when memory runs short, a
memory leak fixed, fewer waits in the GPU thread and frame generation, and online play through Nextendo.

> [!IMPORTANT]
> This is an unofficial fork, not an official Lichii release. Please report problems [here](https://github.com/elkarbeenus/LakuenPlay/issues), not upstream. Games, keys and firmware are not included: only use content you have legally obtained.

## Downloads

| File | What it is |
| --- | --- |
| `LakuenPlay-v1.0.8-Handheld-x86_64-pgo.AppImage` | **Recommended.** Self-contained, compiled for Zen 2 and later with profile-guided optimization (a profile recorded on a Steam Deck) |
| `LakuenPlay-v1.0.8-Handheld-x86_64.AppImage` | The same program without PGO |
| `*.tar.gz` | Plain binaries that use your system libraries |
| `SHA256SUMS.txt` | Checksums |

## From Lichii (16-17 November)

- **When memory runs short:** a buffer that gets no memory frees staging memory and tries again instead of aborting
  ([#1963](https://github.com/zortos293/Lichii/pull/1963), [#1931](https://github.com/zortos293/Lichii/pull/1931)); no submit
  in the middle of a draw then ([#1997](https://github.com/zortos293/Lichii/pull/1997)); deferred staging buffers stay out
  of the pool ([#1906](https://github.com/zortos293/Lichii/pull/1906)).
- **A memory leak fixed:** a compute upload list grew for ever ([#1908](https://github.com/zortos293/Lichii/pull/1908)).
- **No device loss on AMD at 1.5x / 2x:** images are not scaled past the GPU's limits
  ([#1890](https://github.com/zortos293/Lichii/pull/1890)).
- **Fewer waits:** frame generation never waits for a frame to be shown ([#2023](https://github.com/zortos293/Lichii/pull/2023));
  the GPU thread never waits for the game's turn ([#1980](https://github.com/zortos293/Lichii/pull/1980)), with fewer locks
  ([#1953](https://github.com/zortos293/Lichii/pull/1953)); an unused anti-aliasing pass is skipped
  ([#1941](https://github.com/zortos293/Lichii/pull/1941)).
- **Stopping and errors:** a lost GPU device ends the program cleanly with a word in the log
  ([#1938](https://github.com/zortos293/Lichii/pull/1938)); safer stops ([#1957](https://github.com/zortos293/Lichii/pull/1957),
  [#1952](https://github.com/zortos293/Lichii/pull/1952), [#1932](https://github.com/zortos293/Lichii/pull/1932)); RADV's GPU
  hang report kept with `RADV_DEBUG=hang` ([#2050](https://github.com/zortos293/Lichii/pull/2050)).
- **Online play through Nextendo** ([#1979](https://github.com/zortos293/Lichii/pull/1979)), Help > Check for Updates from
  upstream ([#1910](https://github.com/zortos293/Lichii/pull/1910)), the output pass at No sharpening
  ([#1875](https://github.com/zortos293/Lichii/pull/1875)), padded depth targets ([#1868](https://github.com/zortos293/Lichii/pull/1868)),
  and many loader, crypto and network fixes.

This fork's handheld work (memory guard, low-RAM texture limit, heap trimming, pipeline eviction, the recommended handheld
settings, the master log, updates from this repository) stays as in v1.0.7.

## Credits

Built on **[Lichii](https://github.com/zortos293/Lichii)** (zortos293 and contributors), **[Eglantine](https://github.com/Djihads80/Eglantine)**, **Eden**, **yuzu**, **Sudachi** and **Citra**, with work from [Fritchii](https://github.com/Frizti/Fritchii). Thank you!
