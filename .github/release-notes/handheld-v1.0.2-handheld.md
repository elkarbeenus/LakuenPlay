**LaküenPlay v1.0.2 – Handheld** fixes a freeze: v1.0 and v1.0.1 could hang a whole Steam Deck. Please use this version instead.

> [!IMPORTANT]
> This is an unofficial fork, not an official Lichii release. Please report problems [here](https://github.com/elkarbeenus/LakuenPlay/issues), not upstream. Games, keys and firmware are not included: only use content you have legally obtained.

## Downloads

| File | What it is |
| --- | --- |
| `LakuenPlay-v1.0.2-Handheld-x86_64-pgo.AppImage` | **Recommended.** Self-contained, compiled for Zen 2 and later with profile-guided optimization (a profile recorded on a Steam Deck) |
| `LakuenPlay-v1.0.2-Handheld-x86_64.AppImage` | The same program without PGO |
| `*.tar.gz` | Plain binaries that use your system libraries |
| `SHA256SUMS.txt` | Checksums |

## What's new since v1.0.1

- **The Linux pass that frees idle all-zero guest pages is off by default.** On the new Lichii base it froze a Steam Deck (the same pass on the old build-130 base did not; the new base hands guest memory to the GPU and manages it itself). `EDEN_NX2_ZERO_TRIM=1` turns it on again for testing.
- The memory guard, the texture limits and the heap trimming stay on.

Everything else is as in [v1.0.1](https://github.com/elkarbeenus/LakuenPlay/releases/tag/handheld-v1.0.1).

## Credits

Built on **[Lichii](https://github.com/zortos293/Lichii)** (zortos293 and contributors), **[Eglantine](https://github.com/Djihads80/Eglantine)**, **Eden**, **yuzu**, **Sudachi** and **Citra**, with work from [Fritchii](https://github.com/Frizti/Fritchii). Thank you!

Licensed under GPL-3.0-or-later. Not affiliated with Nintendo.
