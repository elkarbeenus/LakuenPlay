> [!WARNING]
> **Test build.** This release is for testing on the Steam Deck and the Legion Go S and will be replaced by a regular
> release once it is confirmed.

> [!CAUTION]
> **Coming from v1.0.4 or older: download this one by hand, once, and do not accept the update those versions offer.**
> Their update check still looked at Lichii's releases and would replace LaküenPlay with Lichii (without this fork's
> handheld work). From v1.0.5 on, LaküenPlay updates itself from this repository: it says when a newer LaküenPlay is out,
> downloads it, replaces itself and starts again.

**LaküenPlay v1.0.5 – Handheld** brings Lichii's build 228 (fewer waits for the GPU, faster shader loading, ASTC on
integrated GPUs) and updates inside the program.

> [!IMPORTANT]
> This is an unofficial fork, not an official Lichii release. Please report problems [here](https://github.com/elkarbeenus/LakuenPlay/issues), not upstream. Games, keys and firmware are not included: only use content you have legally obtained.

## Downloads

| File | What it is |
| --- | --- |
| `LakuenPlay-v1.0.5-Handheld-x86_64-pgo.AppImage` | **Recommended.** Self-contained, compiled for Zen 2 and later with profile-guided optimization (a profile recorded on a Steam Deck) |
| `LakuenPlay-v1.0.5-Handheld-x86_64.AppImage` | The same program without PGO |
| `*.tar.gz` | Plain binaries that use your system libraries |
| `SHA256SUMS.txt` | Checksums |

## New in this fork

- **Updates inside LaküenPlay:** at start it looks at this repository's releases of its own kind (Handheld updates to
  Handheld, PC to PC), offers a newer version, downloads the matching AppImage (PGO or not, as now), checks it, replaces
  itself, keeps the previous version for one rollback and starts again. The "Check for updates" setting turns
  it off.

## What's new since v1.0.4 (from Lichii, build 228)

- **Fewer waits for the GPU:** compute results the game reads back every few frames are downloaded early
  ([#1795](https://github.com/zortos293/Lichii/pull/1795)).
- **Faster start:** Loading Shaders waits only for the last two sessions' pipelines, older ones build in the background
  ([#1803](https://github.com/zortos293/Lichii/pull/1803)).
- **Integrated GPUs (Steam Deck, Legion Go S):** large ASTC arrays are decoded in batches
  ([#1823](https://github.com/zortos293/Lichii/pull/1823)); the ASTC-to-BC transcode keeps its staging buffers until it
  has run ([#1796](https://github.com/zortos293/Lichii/pull/1796)).
- **Less CPU:** write traps skip the texture cache on pages without images
  ([#1789](https://github.com/zortos293/Lichii/pull/1789)); no submit in the middle of a draw
  ([#1812](https://github.com/zortos293/Lichii/pull/1812)); the GPU thread's split runs at High accuracy too
  ([#1808](https://github.com/zortos293/Lichii/pull/1808)).
- **Stability:** occlusion query reads bounded after a silent GPU reset
  ([#1809](https://github.com/zortos293/Lichii/pull/1809)), memory span fixes
  ([#1783](https://github.com/zortos293/Lichii/pull/1783), [#1794](https://github.com/zortos293/Lichii/pull/1794)).
- **The game:** clothes and hair sway at real speed above 60 fps ([#1793](https://github.com/zortos293/Lichii/pull/1793)),
  more 1.0 output shaders recognized ([#1778](https://github.com/zortos293/Lichii/pull/1778)), frame generation spaced from
  its deadline ([#1779](https://github.com/zortos293/Lichii/pull/1779)), game files opened on first use
  ([#1784](https://github.com/zortos293/Lichii/pull/1784)).

This fork's handheld work (memory guard, texture limits, heap trimming, pipeline eviction when memory is critical, the
recommended handheld settings, the master log, Save Data Transfer, crash logs, LaküenPlay's name) stays as in v1.0.4.

## Credits

Built on **[Lichii](https://github.com/zortos293/Lichii)** (zortos293 and contributors), **[Eglantine](https://github.com/Djihads80/Eglantine)**, **Eden**, **yuzu**, **Sudachi** and **Citra**, with work from [Fritchii](https://github.com/Frizti/Fritchii). Thank you!
