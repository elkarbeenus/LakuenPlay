> [!WARNING]
> **Test build.** This release is for testing on the Steam Deck and the Legion Go S and will be replaced by a regular
> release once it is confirmed.

> [!NOTE]
> **From v1.0.6 this update arrives by itself:** LaküenPlay says at start that v1.0.7 is out, downloads it, replaces
> itself and starts again. Coming from v1.0.5 or older, download this one by hand once.

**LaküenPlay v1.0.7 – Handheld** brings Lichii's fixes of 15 November after build 228 and a menu entry to check for
updates by hand.

> [!IMPORTANT]
> This is an unofficial fork, not an official Lichii release. Please report problems [here](https://github.com/elkarbeenus/LakuenPlay/issues), not upstream. Games, keys and firmware are not included: only use content you have legally obtained.

## Downloads

| File | What it is |
| --- | --- |
| `LakuenPlay-v1.0.7-Handheld-x86_64-pgo.AppImage` | **Recommended.** Self-contained, compiled for Zen 2 and later with profile-guided optimization (a profile recorded on a Steam Deck) |
| `LakuenPlay-v1.0.7-Handheld-x86_64.AppImage` | The same program without PGO |
| `*.tar.gz` | Plain binaries that use your system libraries |
| `SHA256SUMS.txt` | Checksums |

## New in this fork

- **Help > Check for Updates...:** the start-up check stays silent unless a newer LaküenPlay is out; the menu entry
  checks at once and also says when this version is the newest, or why GitHub could not be asked.

## From Lichii (after build 228)

- **Less video memory for overwritten render targets:** images that newer ones partly overwrote were kept for 150
  frames, in one place 16.8 GB of them and 5 fps. They are now collected at the normal age
  ([#1845](https://github.com/zortos293/Lichii/pull/1845)). This fork's own memory guard and heap trimming stay as they are.
- **FSR and DLSS stay on:** the upscale passes are recognized by a fixed program key, so a shader change can no longer
  turn the upscaler off without a word; one case was found on RADV in handheld mode
  ([#1871](https://github.com/zortos293/Lichii/pull/1871)).
- **No more silent game after starting at volume 0** ([#1847](https://github.com/zortos293/Lichii/pull/1847)).
- **The system's Vulkan loader is used,** not a bundled one, which could fail Vulkan start-up
  ([#1854](https://github.com/zortos293/Lichii/pull/1854)).
- Also: exact QMD reductions ([#1850](https://github.com/zortos293/Lichii/pull/1850)), GPU fixes for NVIDIA
  ([#1848](https://github.com/zortos293/Lichii/pull/1848)) and Intel ([#1853](https://github.com/zortos293/Lichii/pull/1853)),
  empty XDG directories ignored ([#1846](https://github.com/zortos293/Lichii/pull/1846)), an upload trace for debugging
  ([#1865](https://github.com/zortos293/Lichii/pull/1865)), test fixes.

## Credits

Built on **[Lichii](https://github.com/zortos293/Lichii)** (zortos293 and contributors), **[Eglantine](https://github.com/Djihads80/Eglantine)**, **Eden**, **yuzu**, **Sudachi** and **Citra**, with work from [Fritchii](https://github.com/Frizti/Fritchii). Thank you!
