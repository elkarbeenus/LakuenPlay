<!--
# SPDX-FileCopyrightText: Copyright 2025 Eden Emulator Project
# SPDX-License-Identifier: GPL-3.0-or-later

# SPDX-FileCopyrightText: 2018 yuzu Emulator Project
# SPDX-License-Identifier: GPL-2.0-or-later
-->

<h1 align="center">LaküenPlay</h1>

<p align="center">
  <b>A performance-focused Linux fork of <a href="https://github.com/zortos293/Lichii">Lichii</a> build 130</b><br>
  Tuned for desktop PCs and AMD handhelds (Steam Deck, Lenovo Legion Go S)
</p>

<p align="center">
  <img alt="License: GPL-3.0-or-later" src="https://img.shields.io/badge/license-GPL--3.0--or--later-blue">
  <img alt="Platform: Linux x86-64" src="https://img.shields.io/badge/platform-Linux%20x86--64-informational">
  <img alt="Base: Lichii build 130" src="https://img.shields.io/badge/base-Lichii%20build%20130-8a63d2">
  <img alt="Status: experimental" src="https://img.shields.io/badge/status-experimental-orange">
</p>

---

LaküenPlay is an unofficial, experimental fork of **Lichii**, the open-source Nintendo Switch and Switch 2 emulator. The PC build starts from Lichii **build 130**, the handheld build from a current Lichii (`nx2`, past build 160); both add performance, memory and stability work for Linux. It focuses on two targets: a desktop build, and a handheld build for AMD's low-power APUs.

Almost everything here builds on other people's work. Lichii, Eglantine, Eden, yuzu and Sudachi did the hard part; this fork collects, ports and tunes. See [Credits](#credits).

> [!IMPORTANT]
> This is **not** an official Lichii release and is not affiliated with the Lichii, Eglantine or Eden projects, or with Nintendo. Please report problems with these builds **here**, not upstream. For the official emulator, use [Lichii's releases](https://github.com/zortos293/Lichii/releases).

## Contents

- [Builds](#builds)
- [What changed since Lichii build 130](#what-changed-since-lichii-build-130)
- [Handheld build](#handheld-build)
- [Environment switches](#environment-switches)
- [Installing and data folders](#installing-and-data-folders)
- [Building from source](#building-from-source)
- [Credits](#credits)
- [License](#license)

## Builds

| Build | Branch | Target hardware | CPU baseline |
| --- | --- | --- | --- |
| **PC** | [`build_130_better_ram`](../../tree/build_130_better_ram) | Desktop Linux / SteamOS PCs | generic x86-64 |
| **Handheld** | [`build_nx2_handheld_v11`](../../tree/build_nx2_handheld_v11) (base: Lichii `nx2` of 2026-11-13, past build 160) | Steam Deck (LCD/OLED), Legion Go S (Z2 Go) and similar AMD handhelds | Zen 2 and newer |

Each build ships as a self-contained **AppImage** in two variants:

- **PGO** (recommended): the same program compiled with profile-guided optimization, using a profile recorded on the target hardware.
- **Standard**: no profile, for comparison or if the PGO build misbehaves.

**⬇️ Latest releases:**
- **PC:** [LaküenPlay v1.2 – PC](../../releases/tag/v1.2) (`LakuenPlay-v1.2-PC-x86_64-pgo.AppImage` is recommended)
- **Handheld:** [LaküenPlay v1.0.2 – Handheld](../../releases/tag/handheld-v1.0.2) (`LakuenPlay-v1.0.2-Handheld-x86_64-pgo.AppImage` is recommended; Steam Deck, Legion Go S and other Zen 2+ handhelds)

### What LaküenPlay adds

Compared with [Lichii](https://github.com/zortos293/Lichii) itself:

| | LaküenPlay PC (v1.2) | LaküenPlay Handheld (v1.0.2) | Lichii |
|---|:-:|:-:|:-:|
| Memory guard for 16 GB shared RAM (caches give memory back below 2.5 / 1.5 GiB free) | – | ✅ | – |
| Texture cache capped at 3–3.5 GiB on 16 GB machines, also on integrated GPUs | ✅ | ✅ | – |
| Freed heap handed back to Linux after the boot builds and evictions | ✅ | ✅ | – |
| Idle all-zero guest memory freed on Linux (Lichii: Windows only) | ✅ | ⚙️ off by default | – |
| Shader cache file mapped instead of copied into RAM (Linux) | ✅ | ✅ | – |
| Pipeline misses during play translate their stages in parallel | ✅ | ✅ | – |
| Compiled for the device's CPU (Zen 2, Zen 3 for the Legion Go S) with a PGO profile recorded on that hardware | Ryzen 7 5800X3D profile | Steam Deck profile | generic x86-64 |
| Frame generation held at 2x with evenly spaced frames | up to 4x, as Lichii | ✅ | up to 4x |
| Save Data Transfer (a game's save as a zip, backed up before an import) | ✅ | ✅ | – |
| The log written out on a crash or a GPU device loss | ✅ | ✅ | – |
| Linux-only CI: AppImages for desktop and handheld, no other platforms | ✅ | ✅ | Windows, Linux, macOS, Android |

⚙️ `EDEN_NX2_ZERO_TRIM=1` turns it on. Every change can be switched off with an environment variable.

Test builds are produced by [GitHub Actions](../../actions/workflows/release-binaries.yml), and every run is titled with what it contains. Downloading an artifact needs a GitHub login.

## What changed since Lichii build 130

The changes are grouped by area. Each one names where it came from. Nearly every new behaviour has an environment switch, so it can be turned off for comparison (see [Environment switches](#environment-switches)).

### GPU thread and command processing

- **Split GPU thread with fewer stalls** ([Lichii #1161](https://github.com/zortos293/Lichii/pull/1161)). Semaphore acquires are decided without draining the split, delta "fast draws" are taken in more cases, and reports are cheaper on the split's back thread. Synchronization waits dropped to roughly a quarter.
- **Fast draws survive more register writes.** Writes to `vertex_id_base`, counter resets, render-enable and per-draw reports no longer force a full draw setup.
- **Empty submits are skipped.** `FlushCommands` no longer submits to the GPU when nothing was recorded since the last submit (from Fritchii). Fewer driver calls and less CPU time.
- **Faster JIT compilation.** Dynarmic's IR verification pass is skipped in release builds, so guest code compiles faster when new code paths come up.
- **GPU page translations cached per thread** (from Eglantine). This cuts repeated page-table walks on the GPU threads.
- **Faster shader translation for large compute programs.** Storage-buffer searches don't walk the same failed instructions again (from Eglantine).

### Memory and RAM usage

- **All-zero guest pages are handed back to the system.** Games often clear several GB of heap at boot. On Linux these pages are punched out of the backing memory, and the emulator's freed heap is returned with `malloc_trim`.
- **The pipeline cache keeps what it built.** The budget for unused prebuilt pipelines scales with video memory, between 2,000 and 10,000, and the idle age before eviction is ~7.5 min at 60 fps instead of ~2.5 min ([Lichii #1107](https://github.com/zortos293/Lichii/pull/1107)). This avoids rebuild stutter when you return to an area.
- **Integrated GPUs stay out of permanent "texture emergency" mode** ([Lichii #823](https://github.com/zortos293/Lichii/pull/823)). The texture budget on iGPUs now limits textures only, not all GPU memory.
- **Uneven-mip ASTC textures stay compressed on AMD** ([Lichii #1194](https://github.com/zortos293/Lichii/pull/1194)). They are resized to an evenly halving shape and re-compressed to BC instead of kept as RGBA8, which is 4–8× smaller.
- **At most 4 shader workers on Linux PCs with 16 GB of RAM or less.** This prevents memory spikes during shader bursts.
- **"Hot pages"** (from Eglantine) stops write-protecting pages the CPU rewrites every frame. It is available but **off by default**, because it raised RAM use on some systems.

### AMD / RADV stability

- **Task/mesh draws take the compute fallback on RADV by default.** Native mesh shaders hang RDNA2 GPUs on current Mesa ([Lichii #871](https://github.com/zortos293/Lichii/issues/871)).
- **Task/mesh pipelines are built without the driver's optimizer on AMD.** This avoids pathological compile times.
- **Bounds-checked texture decodes and robust buffer access** against AMD device-lost errors.
- **A ceiling on compare-exchange loop attempts in shaders**, against GPU hangs.
- **The leaner mesh-emulation path** (from lethevimlet's [BC-250 fork](https://github.com/lethevimlet/oot-bc250)) skips empty emulated mesh draws and uses a narrower barrier.
- **The occlusion-query copy waits only on the newest query on RADV too**, as Lichii already did on AMD's proprietary driver.

### Stability fixes ported from newer Lichii

- **No more freeze while a compute pipeline is rebuilt** ([Lichii #969](https://github.com/zortos293/Lichii/pull/969)). Dispatches keep running while a native compute pipeline is rebuilt.
- **Evicted pipelines keep their storage-buffer pointers when translated again** ([#1143](https://github.com/zortos293/Lichii/pull/1143), [#1154](https://github.com/zortos293/Lichii/pull/1154)).
- **Destroyed memory releases its GPU ranges** ([#1199](https://github.com/zortos293/Lichii/pull/1199), [#1221](https://github.com/zortos293/Lichii/pull/1221)). Every GPU range into memory that is destroyed is unmapped and its device range released. A reused device range is mapped to the CPU range's current pages. PC build only.
- **Fewer lock-order deadlocks** ([#1218](https://github.com/zortos293/Lichii/pull/1218)). GPU-memory registry and address-space listeners are called outside their locks. PC build only.
- **No UI freeze when closing a stuck game** ([#1234](https://github.com/zortos293/Lichii/pull/1234)).
- **No ADPCM audio crackle** ([#1196](https://github.com/zortos293/Lichii/pull/1196)). The sign of the ADPCM nibble steps is fixed.
- **No wasted depth upload.** A cleared depth target that is larger than its colour target no longer gets a guest upload.

### Build and tooling

- **Linux-only CI with PGO.** `clang` with profile-guided optimization, using profiles recorded on a Ryzen 7 5800X3D / RX 6600 XT (PC) and a Steam Deck (handheld).
- **Profiles can be recorded on your own device.** An instrumented "measurement" build mode (`pgo_mode=generate`) writes them, and raw profiles in `.zst` or `.xz` are merged automatically.
- **Normal and instrumented builds of a branch can run side by side.** Each run carries a short note in its title.
- **Clear naming.** The window title, log header and About dialog name the build and credit Lichii and Eden.

## Handheld build

On top of everything above, the handheld build adds:

- **Zen 2 compile target.** Built with the `zen2` preset for the Steam Deck APU and Legion Go S. It does not run on older CPUs.
- **A dedicated core for the GPU back thread on 4–5-core CPUs.** Lichii only pinned threads from 6 cores up. Here the back thread gets the last physical core to itself.
- **Frame-ahead presentation.** The game may start the next frame while the GPU side is still behind, so CPU, GPU threads and GPU overlap (from lethevimlet's BC-250 fork).
- **Recommended settings for handhelds.** Steam Deck and AMD Ryzen Z1/Z2 handhelds are recognized by CPU name, and the build offers fitting settings once: handheld mode, 1× resolution, no FXAA, bilinear scaling, normal GPU clocks and shader precaching before game start. You can apply them later with *Help → Apply Recommended Settings* and undo them with *Help → Restore My Previous Settings* (built on [Lichii #1200](https://github.com/zortos293/Lichii/pull/1200)).
- **More fast draws.** Writes to render-target slots the current draw doesn't use no longer end a fast-draw chain. On a Steam Deck this raised delta draws by about a third.
- **Optional `malloc_trim` skip** once there is nothing left to trim, to test against periodic hitches.

## Environment switches

Set these before launching. For example, in Steam's launch options: `EDEN_NX2_HOT_PAGES=1 %command%`.

| Variable | Default | Effect |
| --- | --- | --- |
| `EDEN_NX2_HOT_PAGES=1` | off | Stop write-protecting pages the CPU rewrites every frame |
| `EDEN_NX2_NO_TRANSLATION_CACHE=1` | cache on | Disable the per-thread GPU page translation cache |
| `EDEN_NX2_EMPTY_FLUSH=1` | skip on | Submit to the GPU even when nothing was recorded |
| `EDEN_NX2_JIT_FAST_COMPILE=0` | on | Keep dynarmic's IR verification pass |
| `EDEN_NX2_DEPTH_CLEAR_UPLOAD=1` | skip on | Upload cleared depth targets again |
| `EDEN_NX2_EVEN_BC=0` | on | Keep uneven-mip ASTC textures as RGBA8 on AMD |
| `EDEN_NX2_QUERY_WAIT_ALL=1` | off | Wait on every occlusion query on RADV |
| `EDEN_NX2_MESH_WIDE_BARRIER=1` | off | Use the full barrier before emulated mesh draws |
| `EDEN_NX2_FORCE_MESH_FALLBACK=0` | fallback on RADV | Use native task/mesh shaders on RADV (may hang RDNA2) |
| `EDEN_NX2_AMD_MESH_OPT=1` | off | Build task/mesh pipelines with the driver's optimizer on AMD |
| `EDEN_NX2_ZERO_TRIM=0` | on | Don't return all-zero guest pages and freed heap to the system |
| `EDEN_NX2_PIN_BACK_CORE=0` | on (handheld) | Don't pin the GPU back thread on 4–5-core CPUs |
| `EDEN_NX2_FRAMES_AHEAD=1` | 2 (handheld) | Don't run a frame ahead |
| `EDEN_NX2_FAST_DRAW_RT_SLOTS=0` | on (handheld) | Let writes to unused render-target slots end fast draws again |
| `EDEN_NX2_MALLOC_TRIM=0` | off (handheld) | Skip `malloc_trim` once nothing is left to trim |

## Installing and data folders

1. Download the AppImage for your device (see [Builds](#builds)).
2. Make it executable (`chmod +x *.AppImage`) and run it. On SteamOS you can add it to Steam as a non-Steam game.
3. Supply your own keys, firmware and games. Only use content you have legally obtained. Nothing of the kind is included or provided here.

LaküenPlay uses **the same data folders and settings as Lichii**. Saves, keys, settings and shader caches carry over in both directions. Back up your saves before trying experimental builds.

## Building from source

The build steps are the same as Lichii's. See the [build guide](docs/Build.md). The exact CI configuration, including the PGO steps, is in [`.github/workflows/release-binaries.yml`](.github/workflows/release-binaries.yml). The GUI target is still called `yuzu`, and the binary is `lichii`.

## Credits

LaküenPlay exists because of these projects and people:

- **[Lichii](https://github.com/zortos293/Lichii)** by zortos293 and contributors: the base of this fork (build 130) and most of the ported fixes.
- **[Eglantine](https://github.com/Djihads80/Eglantine)**: hot pages, the GPU page translation cache and faster compute-shader translation.
- **Eden**, **yuzu**, **Sudachi** and **Citra**: the emulator foundations underneath all of the above.
- **Lichii pull request authors** whose work is included: Lucas Borges (#1161), Brandon Bunnelle (#1107), Nanoc6 (#1218), PantACRO4life (#1221), Daniel Jesús Martín Flores (#1234), Zachary McKenney (#1154), Zortos (#1143, #1199, #1200), and the authors of #823, #969, #1194 and #1196.
- **[Fritchii](https://github.com/Frizti/Lichii)** by Frizti: skipping empty GPU submits.
- **[lethevimlet's BC-250 fork](https://github.com/lethevimlet/oot-bc250)**: the leaner mesh-emulation path and frame-ahead presentation.
- **[Lichii Ally](https://github.com/Syyrhc/Lichii-Ally)** by Syyrhc: the blit path for skipping a game's post-processing network.

Changes made in this fork were developed with the help of Claude (Anthropic). Each commit names its source.

## License

LaküenPlay is licensed under the **GNU General Public License v3.0 or later**, the same as Lichii and Eden (see [LICENSE.txt](LICENSE.txt)). Parts inherited from yuzu and Citra are GPL-2.0-or-later. Per-file license details are in [`LICENSES/`](LICENSES) and the SPDX headers.

*Nintendo Switch is a trademark of Nintendo. This project is not affiliated with or endorsed by Nintendo.*
