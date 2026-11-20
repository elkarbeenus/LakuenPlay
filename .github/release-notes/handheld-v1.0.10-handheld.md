> [!WARNING]
> **Test build.** This release is for testing on the Steam Deck and the Legion Go S and will be replaced by a regular
> release once it is confirmed.

> [!NOTE]
> **From v1.0.6 this update arrives by itself:** LaküenPlay says at start that v1.0.10 is out, downloads it, replaces
> itself and starts again. Coming from v1.0.5 or older, download this one by hand once.

**LaküenPlay v1.0.10 – Handheld** brings Lichii's work of 19 and 20 November: stability fixes in the texture
cache, the timers and the audio output.

> [!IMPORTANT]
> This is an unofficial fork, not an official Lichii release. Please report problems [here](https://github.com/elkarbeenus/LakuenPlay/issues), not upstream. Games, keys and firmware are not included: only use content you have legally obtained.

## Downloads

| File | What it is |
| --- | --- |
| `LakuenPlay-v1.0.10-Handheld-x86_64-pgo.AppImage` | **Recommended.** Self-contained, compiled for Zen 2 and later with profile-guided optimization (a profile recorded on a Steam Deck) |
| `LakuenPlay-v1.0.10-Handheld-x86_64.AppImage` | The same program without PGO |
| `*.tar.gz` | Plain binaries that use your system libraries |
| `SHA256SUMS.txt` | Checksums |

## From Lichii (19-20 November)

- **Stability:** the texture cache recovers when a host scratch allocation fails instead of going on with a broken image
  ([#2290](https://github.com/zortos293/Lichii/pull/2290)); timer events whose owner is gone are dropped instead of run
  ([#2252](https://github.com/zortos293/Lichii/pull/2252)); audio output streams are purged with the rest
  ([#2299](https://github.com/zortos293/Lichii/pull/2299)); a first failing compute macro is captured for reports
  ([#2253](https://github.com/zortos293/Lichii/pull/2253)).
- Also: the HOME Menu's own work, saves kept when linking another emulator's saves
  ([#2245](https://github.com/zortos293/Lichii/pull/2245)), loopback fixes for online play
  ([#2187](https://github.com/zortos293/Lichii/pull/2187)), and fixes for other games.

This fork's handheld work (memory guard, low-RAM texture limit, heap trimming, pipeline eviction, the depth clear without
an upload, the recommended handheld settings, the master log, updates from this repository) stays as in v1.0.9.

## Credits

Built on **[Lichii](https://github.com/zortos293/Lichii)** (zortos293 and contributors), **[Eglantine](https://github.com/Djihads80/Eglantine)**, **Eden**, **yuzu**, **Sudachi** and **Citra**, with work from [Fritchii](https://github.com/Frizti/Fritchii). Thank you!
