// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <string>
#include <string_view>
#include <type_traits>
#include <vector>
#include "common/settings.h"

namespace Settings {

/// (this fork) Handhelds the recommended table has its own values for.
enum class HandheldDevice { None, SteamDeck, AmdHandheld };

/// The handheld this host is, from the CPU's name: Steam Deck LCD and OLED ("AMD Custom APU 0405"
/// and "0932"), Legion Go S and other AMD handhelds ("AMD Ryzen Z1" / "Z2"). None elsewhere.
[[nodiscard]] HandheldDevice DetectHandheldDevice();

/// "Steam Deck", "Legion Go S / AMD handheld", or an empty string.
[[nodiscard]] std::string_view HandheldDeviceName(HandheldDevice device);

/// The Lichii developer's recommended settings. This table is the only place that lists them:
/// change a value here to change what "Use recommended settings" writes. Personal and hardware
/// choices (language, region, resolution, upscaler, backend, device) stay out of it.
template <typename Func>
void ForEachRecommendedSetting(Values& values, Func&& func) {
    // (this fork) On a Steam Deck or an AMD handheld (Legion Go S), the values measured there:
    // the CPU side of the emulator is the limit, so the game may lower its resolution and render
    // distance (GPU Clocks Normal), and the handheld renders in handheld mode without FXAA.
    const bool handheld = DetectHandheldDevice() != HandheldDevice::None;
    const auto entry = [&func]<typename Type, bool ranged>(SwitchableSetting<Type, ranged>& setting,
                                                           std::type_identity_t<Type> value) {
        func(setting, value);
    };

    // Graphics > Advanced
    entry(values.gpu_accuracy, GpuAccuracy::Low);
    entry(values.dma_accuracy, DmaAccuracy::Default);
    entry(values.gpu_fence_behavior, GpuFenceBehavior::Default);
    entry(values.native_compute_write_mark, NativeComputeWriteMark::Auto);
    entry(values.vram_usage_mode, VramUsageMode::Conservative);
    entry(values.nvdec_emulation, NvdecEmulation::Gpu);
    entry(values.max_anisotropy, AnisotropyMode::Automatic);
    entry(values.accelerate_astc, AstcDecodeMode::Gpu);
    entry(values.frame_pacing_mode, FramePacingMode::Target_Auto);
    entry(values.astc_recompression, AstcRecompression::Bc3);
    entry(values.sync_memory_operations, false);
    entry(values.renderer_force_max_clock, false);
    entry(values.use_disk_shader_cache, true);
    entry(values.use_vulkan_driver_pipeline_cache, true);
    entry(values.native_async_draws, NativeAsyncDraws::Bounded);
    entry(values.native_pipeline_wait_ms, 8);
    entry(values.precache_shaders, PrecacheShaders::Off);
    entry(values.use_video_framerate, false);
    entry(values.mkw_30fps, false);
    entry(values.mkw_dynamic_fps, false);
    entry(values.use_reactive_flushing, true);
    entry(values.barrier_feedback_loops, true);
    entry(values.enable_buffer_history, false);

    // Graphics > Extras
    entry(values.skip_cpu_inner_invalidation, false);
    entry(values.async_presentation, true);
    entry(values.fix_bloom_effects, false);
    entry(values.use_asynchronous_shaders, true);
    entry(values.gpu_unswizzle_texture_size, GpuUnswizzleSize::Large);
    entry(values.gpu_unswizzle_stream_size, GpuUnswizzle::Medium);
    entry(values.gpu_unswizzle_chunk_size, GpuUnswizzleChunk::Medium);
    entry(values.gpu_unswizzle_enabled, false);
#if defined(_WIN32) || defined(__linux__)
    // Other hosts skip the pinning.
    entry(values.pin_gpu_threads, true);
#endif
#ifndef __APPLE__
    // Extended dynamic state stays disabled on macOS: MoltenVK renders black screens with it.
    entry(values.dyna_state, ExtendedDynamicState::EDS2);
#endif
    entry(values.sample_shading, 0);
    entry(values.vertex_input_dynamic_state, true);

    // System
    entry(values.cpu_clock, CpuClock::Normal);
    entry(values.gpu_clock, handheld ? GpuClock::Normal : GpuClock::Boost);
    if (handheld) {
        entry(values.use_docked_mode, ConsoleMode::Handheld);
        entry(values.resolution_setup, ResolutionSetup::Res1X);
        entry(values.anti_aliasing, AntiAliasing::None);
        entry(values.scaling_filter, ScalingFilter::Bilinear);
        entry(values.oot_anti_aliasing, OotAntiAliasing::NoSharpening);
    }
    entry(values.use_multi_core, true);
    entry(values.memory_layout_mode, MemoryLayout::Memory_4Gb);
    entry(values.use_speed_limit, true);
    entry(values.speed_limit, 100);
    entry(values.slow_speed_limit, 50);
    entry(values.turbo_speed_limit, 200);
    entry(values.sync_core_speed, false);

    // CPU
    entry(values.cpu_accuracy, CpuAccuracy::Auto);
    entry(values.use_custom_cpu_ticks, false);
}

/// A global setting that changed, with its values as BasicSetting::ToString gives them.
struct RecommendedSettingChange {
    const BasicSetting* setting;
    std::string previous;
    std::string current;
};

/// Writes the recommended values to the global settings and returns the ones that changed.
/// Per-game values, and whether a setting uses its per-game value, stay as they are. When
/// something changed, recommended_settings_backup takes the previous value of every recommended
/// setting it does not hold yet. A value it holds stays until it is restored, so applying twice
/// does not lose the values from before the first time.
std::vector<RecommendedSettingChange> ApplyRecommendedSettings(Values& values);

/// Puts back the global values recommended_settings_backup holds, clears it and returns the
/// settings that changed. Only settings in the recommended table are restored.
std::vector<RecommendedSettingChange> RestorePreviousSettings(Values& values);

} // namespace Settings
