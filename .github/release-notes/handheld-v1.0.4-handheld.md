> [!WARNING]
> **Test build.** This release is for testing on the Steam Deck and the Legion Go S and will be replaced by a regular
> release once it is confirmed. Keep [v1.0.3](https://github.com/elkarbeenus/LakuenPlay/releases/tag/handheld-v1.0.3) if
> you need the last confirmed one.

**LaküenPlay v1.0.4 – Handheld** brings about a hundred new Lichii fixes, several of them against crashes and GPU hangs on
handhelds and AMD GPUs, and a master log for diagnosis.

> [!IMPORTANT]
> This is an unofficial fork, not an official Lichii release. Please report problems [here](https://github.com/elkarbeenus/LakuenPlay/issues), not upstream. Games, keys and firmware are not included: only use content you have legally obtained.

## Downloads

| File | What it is |
| --- | --- |
| `LakuenPlay-v1.0.4-Handheld-x86_64-pgo.AppImage` | **Recommended.** Self-contained, compiled for Zen 2 and later with profile-guided optimization (a profile recorded on a Steam Deck) |
| `LakuenPlay-v1.0.4-Handheld-x86_64.AppImage` | The same program without PGO |
| `*.tar.gz` | Plain binaries that use your system libraries |
| `SHA256SUMS.txt` | Checksums |

## What's new since v1.0.3 (from Lichii)

- **AMD RDNA2 (Steam Deck, Legion Go S):** the mesh compute fallback on RADV, against GPU hangs and lost devices
  ([#1732](https://github.com/zortos293/Lichii/pull/1732)).
- **Memory:** the texture cache skips images it cannot evict and gives old memory blocks back
  ([#1731](https://github.com/zortos293/Lichii/pull/1731)); a render target that got no memory is left out instead of
  crashing ([#1766](https://github.com/zortos293/Lichii/pull/1766)); device-local memory placed in host memory is handled
  ([#1686](https://github.com/zortos293/Lichii/pull/1686)).
- **Timing:** a hardware wait just before one-shot events ([#1733](https://github.com/zortos293/Lichii/pull/1733)); fixed
  frame-fence waits suspended in the guest kernel ([#1639](https://github.com/zortos293/Lichii/pull/1639)).
- **Stability:** a stuck stop exits, config and play time are saved atomically
  ([#1728](https://github.com/zortos293/Lichii/pull/1728)); the end-of-load driver cache save no longer races builds
  ([#1685](https://github.com/zortos293/Lichii/pull/1685)); command list pages copied again after DMA copies
  ([#1736](https://github.com/zortos293/Lichii/pull/1736)); the log is flushed every second, so a hang keeps its last lines
  ([#1771](https://github.com/zortos293/Lichii/pull/1771)).
- **The game:** cutscenes and the song recording keep their length with FPS Unlock
  ([#1743](https://github.com/zortos293/Lichii/pull/1743), [#1745](https://github.com/zortos293/Lichii/pull/1745)).
- **Also:** solid NSZ files ([#1734](https://github.com/zortos293/Lichii/pull/1734)), trigger rumble
  ([#1583](https://github.com/zortos293/Lichii/pull/1583)), optional SteamGridDB artwork
  ([#1717](https://github.com/zortos293/Lichii/pull/1717)), and many smaller fixes.
- **Two open Lichii pull requests** for the GPU thread's CPU time: draw state reused on the sole scheduler thread
  ([#1641](https://github.com/zortos293/Lichii/pull/1641)) and an instance loop's repeated draws run at once
  ([#1597](https://github.com/zortos293/Lichii/pull/1597)).

## What's new in this fork

- **Recommended settings for handhelds:** on a Steam Deck or a Legion Go S the recommended settings are Handheld mode,
  1x resolution, no anti-aliasing, bilinear scaling and a 60 fps limit.
- **Master log:** `lakuen_master_log.txt` in the log folder sums up the last five runs (frame
  rate, RAM and swap over the run; `EDEN_NX2_MASTER_LOG=0` turns it off).

This fork's handheld work (memory guard, texture limits, heap trimming, pipeline eviction when memory is critical, frame
generation at 2x, Save Data Transfer, crash logs, LaküenPlay's name) stays as in v1.0.3.

## Credits

Built on **[Lichii](https://github.com/zortos293/Lichii)** (zortos293 and contributors), **[Eglantine](https://github.com/Djihads80/Eglantine)**, **Eden**, **yuzu**, **Sudachi** and **Citra**, with work from [Fritchii](https://github.com/Frizti/Fritchii). Thank you!
