**LaküenPlay v1.0 – Handheld** is the first release of the handheld build: the Linux fork of [Lichii](https://github.com/zortos293/Lichii) for the **Steam Deck** and the **Legion Go S** (AMD APUs), now on a current Lichii base (the `nx2` branch of 2026-11-13, past build 160) instead of build 130.

> [!IMPORTANT]
> This is an unofficial fork, not an official Lichii release. Please report problems [here](https://github.com/elkarbeenus/LakuenPlay/issues), not upstream. Games, keys and firmware are not included: only use content you have legally obtained.

## Downloads

| File | What it is |
| --- | --- |
| `LakuenPlay-v1.0-Handheld-x86_64-pgo.AppImage` | **Recommended.** Self-contained, compiled for Zen 2 and later with profile-guided optimization (a profile recorded on a Steam Deck) |
| `LakuenPlay-v1.0-Handheld-x86_64.AppImage` | The same program without PGO |
| `*.tar.gz` | Plain binaries that use your system libraries |
| `SHA256SUMS.txt` | Checksums |

Make the AppImage executable (`chmod +x LakuenPlay-*.AppImage`) and run it. It needs a Zen 2 CPU or newer (Steam Deck, Legion Go S, ROG Ally and similar).

## From the new Lichii base

- **Faster draws:** fast draws for computed texture handles and dynamic vertex input, reused command list copies, cheaper reports and fences ([Lichii #1515](https://github.com/zortos293/Lichii/pull/1515), [#1516](https://github.com/zortos293/Lichii/pull/1516), [#1519](https://github.com/zortos293/Lichii/pull/1519), [#1520](https://github.com/zortos293/Lichii/pull/1520), [#1521](https://github.com/zortos293/Lichii/pull/1521)).
- **Faster CPU emulation:** better x86-64 code generation ([Lichii #1531](https://github.com/zortos293/Lichii/pull/1531), [#1570](https://github.com/zortos293/Lichii/pull/1570)).
- **Memory:** uploaded guest memory marked cold only when the host is short of RAM ([Lichii #1581](https://github.com/zortos293/Lichii/pull/1581)), freed SMMU blocks reused and given back ([#1528](https://github.com/zortos293/Lichii/pull/1528), [#1532](https://github.com/zortos293/Lichii/pull/1532)).
- **Rendering:** destroyed GPU memory is released and reused device ranges are refreshed, so reused memory no longer shows stale contents ([#1236](https://github.com/zortos293/Lichii/pull/1236)); the dark water grid is fixed ([#1485](https://github.com/zortos293/Lichii/pull/1485)).
- **Stability:** shader cache, teardown and kernel fixes ([#1573](https://github.com/zortos293/Lichii/pull/1573)–[#1576](https://github.com/zortos293/Lichii/pull/1576)), native HDR output ([#1326](https://github.com/zortos293/Lichii/pull/1326)), a redesigned frame statistics overlay ([#1345](https://github.com/zortos293/Lichii/pull/1345)) and LAN play ([#1359](https://github.com/zortos293/Lichii/pull/1359)).

## This fork's handheld changes

### Memory (16 GB shared between the system and the GPU)
- **(Off by default since v1.0.2: it froze a Steam Deck.)** **Idle all-zero guest pages are freed on Linux too** (Lichii frees them on Windows only): games clear several GB of their heaps while booting. A pass also hands the emulator's freed heap back to the system. It comes early after an area change or when RAM runs short.
- **Memory guard:** below 2.5 GiB available the textures collect as if out of memory and the heap is trimmed; below 1.5 GiB pipelines unused for a few minutes go, slowly. Before, a Steam Deck could run out of memory entering an area and the system ended the emulator.
- **Textures stop at 3 GiB** on 15 GiB hosts (Steam Deck), 3.5 GiB on 16 GB machines, also on integrated GPUs, where the cache had grown to 4-5 GiB. An integrated GPU's memory share bounds the textures alone, not everything in device memory ([Lichii #823](https://github.com/zortos293/Lichii/pull/823)).
- **Freed heap goes back to Linux** after the boot builds and evictions (port of [Lichii #1271](https://github.com/zortos293/Lichii/pull/1271)); the texture cache collects faster the further past its limit (port of [#1306](https://github.com/zortos293/Lichii/pull/1306)); the shader cache file is mapped instead of copied into the heap.

### Performance and fixes
- **Frame generation** with evenly spaced frames, **at most 2x**.
- **AMD:** task/mesh draws take the compute fallback on RADV, bounds-checked texture decodes and robust buffer access (device losses), uneven-mip ASTC re-compressed to BC instead of RGBA8, a ceiling on compare-exchange loops (GPU hangs).
- **4-5 core CPUs:** the GPU's back thread gets a physical core alone; GPU page translations are cached per thread; pipeline misses translate their stages in parallel.
- **Save Data Transfer:** right-click a game → *Save Data Transfer* to export its save as a zip or import one (the current save is backed up first).
- **Crash logs** are written out on fatal signals and GPU device loss.

Every change can be switched off with an environment variable (`EDEN_NX2_ZERO_TRIM=0`, `EDEN_NX2_LOW_MEMORY_MIB=0`, `EDEN_NX2_CRITICAL_MEMORY_MIB=0`, `EDEN_NX2_TC_LOW_RAM=0`, ...).

## Credits

Built on **[Lichii](https://github.com/zortos293/Lichii)** (zortos293 and contributors), **[Eglantine](https://github.com/Djihads80/Eglantine)**, **Eden**, **yuzu**, **Sudachi** and **Citra**, with work from [Fritchii](https://github.com/Frizti/Fritchii). Thank you!

Licensed under GPL-3.0-or-later. Not affiliated with Nintendo.
