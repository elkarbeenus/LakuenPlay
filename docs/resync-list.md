# LaküenPlay resync list

Base: `lichii/nx2`, merged in full (last merge: 2026-11-22, build 366). Our delta on top: about 60 files and 1,700 lines.
Before every merge, check that each line below still holds afterwards ("keep our hard optimizations").

## What we have that Lichii has not (keep)

| Area | Change | Switch / note |
|---|---|---|
| Memory | Handheld memory guard: textures and heap give memory back below 2.5 GiB available, unused pipelines go slowly below 1.5 GiB | `memory_detect.cpp`, `device_memory.cpp` |
| Memory | Texture cap 3 GiB on 15 GiB hosts, 3.5 GiB on PCs with 16 GB or less; the low-RAM cap holds on integrated GPUs (`LowRamTextureCap`, kept beside the retention budget) | `texture_cache.h` |
| Memory | Freed heap back to Linux after boot builds and evictions; idle all-zero guest pages (`EDEN_NX2_ZERO_TRIM=1`, off by default) | `host_memory.cpp`, `device_memory.cpp` (Linux trim kept beside upstream's NCE preparation) |
| Memory | `host_low` passed into `RunGarbageCollector(out_of_memory \|\| host_low, spill_collect)` | `texture_cache.h` |
| Memory | Shader cache v2 file mapped instead of copied (Linux) | `shader_cache_v2/cache_file.cpp` |
| GPU (AMD) | Uneven-mip ASTC re-compressed to BC at an even shape on AMD; `TranscodeAstcBatches` with our source/target encode | `vk_texture_cache.cpp`, `astc_decoder.comp`, `bc_encoder.comp` |
| GPU (AMD) | Task/mesh pipelines without the driver's optimizer; compute fallback on RADV by default; empty emulated mesh draws skipped | `vk_pipeline_cache.cpp`, `vk_graphics_pipeline.cpp` |
| GPU (AMD) | Bounds-checked texture decodes, robust buffer access, compare-exchange loop ceiling (GPU hangs) | `spirv_emit_context.cpp`, `vulkan_device.cpp` |
| GPU | Bilinear blit kept in `kepler_compute` | `kepler_compute.cpp` |
| GPU | Per-thread GPU page translation cache; draw state reuse on the sole scheduler thread; repeated draw.begin entries at once | `memory_manager.cpp`, `gpu_split.cpp`, `puller.cpp` |
| GPU | Pipeline cache file written only when its size changed (`FRITCHII_PCACHE_SAVE=0`: as before) | `vk_pipeline_cache.cpp/.h` |
| GPU | Fritchii's depth-clear exception inside upstream's `ClearWritesWhole` (`EDEN_NX2_DEPTH_CLEAR_UPLOAD`) | `texture_cache.h` |
| CPU | On 4-5 core hosts the split's back gets a physical core alone | `thread.cpp` |
| Handheld | Recommended settings for Steam Deck and AMD handhelds; Legion Go S build (Zen 3, own PGO profiles) | `recommended_settings.cpp`, CI |
| App | Name in title, log and About dialog; in-app updates (Help > Check for Updates, release builds by version); master log of the last five runs; crash log on fatal signals and device loss; save data transfer (zip) | `main_window.cpp`, `update_checker.cpp`, `content.cpp` |
| CI | Our `release-binaries.yml` (handheld and PC builds, test builds published as releases) | always `git checkout --ours` on conflicts |

## What others have that we have not

| Source | Change | Status |
|---|---|---|
| Fritchii (v6.x) | Hitch fixes behind its `EDEN_NX2_EXP` experiment system | Watch only; needs the experiment system first |
| Fritchii | JIT shared cache (opt-in), AMD noperspective | Candidates, not ported yet |

## Left out on purpose

- Nothing from NX2navi (not allowed).
- Fritchii's experiment system as a whole (too large, we are ahead on Steam Deck performance).
