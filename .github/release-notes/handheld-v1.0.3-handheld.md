**LaküenPlay v1.0.3 – Handheld** brings the newest Lichii fixes, several of them for handhelds and AMD GPUs.

> [!IMPORTANT]
> This is an unofficial fork, not an official Lichii release. Please report problems [here](https://github.com/elkarbeenus/LakuenPlay/issues), not upstream. Games, keys and firmware are not included: only use content you have legally obtained.

## Downloads

| File | What it is |
| --- | --- |
| `LakuenPlay-v1.0.3-Handheld-x86_64-pgo.AppImage` | **Recommended.** Self-contained, compiled for Zen 2 and later with profile-guided optimization (a profile recorded on a Steam Deck) |
| `LakuenPlay-v1.0.3-Handheld-x86_64.AppImage` | The same program without PGO |
| `*.tar.gz` | Plain binaries that use your system libraries |
| `SHA256SUMS.txt` | Checksums |

## What's new since v1.0.2 (from Lichii)

- **First start with an old shader cache:** a legacy Vulkan cache is migrated without holding it in memory three times ([Lichii #1654](https://github.com/zortos293/Lichii/pull/1654)). On a 16 GB handheld that could fill the RAM at the first start on the new base.
- **AMD (RADV):** the mesh grid size is read correctly on Mesa before 26.2.5, which fixes a GPU hang ([#1203](https://github.com/zortos293/Lichii/pull/1203)); RADV keeps its own pipeline cache ([#1653](https://github.com/zortos293/Lichii/pull/1653)); emulated geometry outputs trimmed (an AMD crash on a loading screen, [#1110](https://github.com/zortos293/Lichii/pull/1110)).
- **Closing a game while shaders load** no longer hangs, and the next launch works ([#1417](https://github.com/zortos293/Lichii/pull/1417)).
- **White flashing** fixed ([#1651](https://github.com/zortos293/Lichii/pull/1651)); water ripples keep running with the FPS patches ([#1180](https://github.com/zortos293/Lichii/pull/1180)).
- **Pipelines:** runtime builds go before the boot prebuild backlog ([#1621](https://github.com/zortos293/Lichii/pull/1621)), the background prebuild builds every skipped cached pipeline, newest first ([#1619](https://github.com/zortos293/Lichii/pull/1619)), texture pools up to 4 MiB are translated on workers ([#1155](https://github.com/zortos293/Lichii/pull/1155)), and the log says what still builds on the GPU thread mid-game ([#1626](https://github.com/zortos293/Lichii/pull/1626)).
- **Memory:** an integrated GPU's memory share bounds the textures only, now upstream ([#823](https://github.com/zortos293/Lichii/pull/823)); cold hints skip placeholder addresses ([#1568](https://github.com/zortos293/Lichii/pull/1568)); installed RAM read from udev on Linux ([#1506](https://github.com/zortos293/Lichii/pull/1506)).
- **Also:** AMD FSR 3.1.6 ([#1624](https://github.com/zortos293/Lichii/pull/1624)), brief audio gaps filled ([#1298](https://github.com/zortos293/Lichii/pull/1298)), and many smaller fixes.

This fork's handheld work (memory guard, texture limits, heap trimming, frame generation at 2x, Save Data Transfer, crash logs, LaküenPlay's name) stays as in [v1.0.2](https://github.com/elkarbeenus/LakuenPlay/releases/tag/handheld-v1.0.2).

## Credits

Built on **[Lichii](https://github.com/zortos293/Lichii)** (zortos293 and contributors), **[Eglantine](https://github.com/Djihads80/Eglantine)**, **Eden**, **yuzu**, **Sudachi** and **Citra**, with work from [Fritchii](https://github.com/Frizti/Fritchii). Thank you!

Licensed under GPL-3.0-or-later. Not affiliated with Nintendo.
