> [!WARNING]
> **Test build.** This release is for testing on the Steam Deck and the Legion Go S and will be replaced by a regular
> release once it is confirmed.

> [!NOTE]
> **From v1.0.6 this update arrives by itself:** LaküenPlay says at start that v1.0.13 is out, downloads it, replaces
> itself and starts again. Coming from v1.0.5 or older, download this one by hand once.

**LaküenPlay v1.0.13 – Handheld** brings three days of Lichii's work: earlier submits while the GPU is idle, less memory for unused buffers,
and a set of stability and shader fixes.

> [!IMPORTANT]
> This is an unofficial fork, not an official Lichii release. Please report problems [here](https://github.com/elkarbeenus/LakuenPlay/issues), not upstream. Games, keys and firmware are not included: only use content you have legally obtained.

## Downloads

| File | What it is |
| --- | --- |
| `LakuenPlay-v1.0.13-Handheld-x86_64-pgo.AppImage` | **Recommended.** Self-contained, compiled for Zen 2 and later with profile-guided optimization (a profile recorded on a Steam Deck) |
| `LakuenPlay-v1.0.13-Handheld-x86_64.AppImage` | The same program without PGO |
| `*.tar.gz` | Plain binaries that use your system libraries |
| `SHA256SUMS.txt` | Checksums |

## From Lichii (23-25 November, builds 366 to 386)

- **Faster on the GPU side:** recorded work is submitted early while the GPU sits idle
  ([#2466](https://github.com/zortos293/Lichii/pull/2466)), native submissions end at render-pass boundaries
  ([#2481](https://github.com/zortos293/Lichii/pull/2481)), render-follower work is skipped on direct GPU paths
  ([#2449](https://github.com/zortos293/Lichii/pull/2449)), and adjacent GPU-written readback pages are read together
  ([#2483](https://github.com/zortos293/Lichii/pull/2483)).
- **Less memory:** buffers unused for 3600 frames are collected even below the 512 MiB trigger
  ([#2465](https://github.com/zortos293/Lichii/pull/2465)); SMAA and FSR 3 share their present images
  ([#2464](https://github.com/zortos293/Lichii/pull/2464)).
- **Stability:** geometry rebound after a submit in the middle of a draw ([#2539](https://github.com/zortos293/Lichii/pull/2539)),
  sparse buffer sources kept until the GPU is done ([#2517](https://github.com/zortos293/Lichii/pull/2517)), retired images
  kept through their delete tick ([#2485](https://github.com/zortos293/Lichii/pull/2485)), view generations allocated before
  retirement ([#2573](https://github.com/zortos293/Lichii/pull/2573)), no repeated shutdown when a game is stopped
  ([#2502](https://github.com/zortos293/Lichii/pull/2502)), presented buffers released through fences
  ([#1640](https://github.com/zortos293/Lichii/pull/1640)).
- **Shaders:** flag operations kept in SPIR-V ([#2523](https://github.com/zortos293/Lichii/pull/2523)), immediate phi operands
  kept defined ([#2567](https://github.com/zortos293/Lichii/pull/2567)), R2P with an immediate mask writes only its predicates
  ([#2577](https://github.com/zortos293/Lichii/pull/2577)).
- Also: an option to keep the window size when a game starts ([#2551](https://github.com/zortos293/Lichii/pull/2551)), Mod
  Store improvements, PNG screenshots in the Album again, and fixes for other games.

This fork's handheld work (memory guard, low-RAM texture limit, heap trimming, pipeline eviction, the depth clear without
an upload, the BC shapes for uneven ASTC mips on AMD, lean pipeline cache saves, the recommended handheld settings, the
master log, updates from this repository) stays as in v1.0.12.

## Credits

Built on **[Lichii](https://github.com/zortos293/Lichii)** (zortos293 and contributors), **[Eglantine](https://github.com/Djihads80/Eglantine)**, **Eden**, **yuzu**, **Sudachi** and **Citra**, with work from [Fritchii](https://github.com/Frizti/Fritchii). Thank you!
