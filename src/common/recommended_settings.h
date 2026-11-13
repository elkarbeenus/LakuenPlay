// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <map>
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
void ForEachRecommendedSetting(Values& settings, Func&& func) {
    // (this fork) On a Steam Deck or an AMD handheld (Legion Go S), the values measured there: the
    // CPU side of the emulator is the limit, so the game may lower its resolution and render
    // distance (GPU Clocks Normal), and the handheld renders in handheld mode without FXAA.
    const bool handheld = DetectHandheldDevice() != HandheldDevice::None;
    const auto entry = [&func]<typename Type, bool ranged>(SwitchableSetting<Type, ranged>& setting,
                                                           std::type_identity_t<Type> value) {
        func(setting, value);
    };

    // Graphics > Advanced
    entry(settings.gpu_accuracy, GpuAccuracy::Low);
    entry(settings.dma_accuracy, DmaAccuracy::Default);
    entry(settings.gpu_fence_behavior, GpuFenceBehavior::Default);
    entry(settings.native_compute_write_mark, NativeComputeWriteMark::Auto);
    entry(settings.vram_usage_mode, VramUsageMode::Conservative);
    entry(settings.nvdec_emulation, NvdecEmulation::Gpu);
    entry(settings.max_anisotropy, AnisotropyMode::Automatic);
    entry(settings.accelerate_astc, AstcDecodeMode::Gpu);
    entry(settings.frame_pacing_mode, FramePacingMode::Target_Auto);
    entry(settings.astc_recompression, AstcRecompression::Bc3);
    entry(settings.sync_memory_operations, false);
    entry(settings.renderer_force_max_clock, false);
    entry(settings.use_disk_shader_cache, true);
    entry(settings.use_vulkan_driver_pipeline_cache, true);
    entry(settings.native_async_draws, NativeAsyncDraws::Bounded);
    entry(settings.native_pipeline_wait_ms, 8);
    entry(settings.precache_shaders, PrecacheShaders::Off);
    entry(settings.use_video_framerate, false);
    entry(settings.mkw_30fps, false);
    entry(settings.mkw_dynamic_fps, false);
    entry(settings.hyrule_dynamic_fps, false);
    entry(settings.hyrule_30fps, false);
    entry(settings.use_reactive_flushing, true);
    entry(settings.barrier_feedback_loops, true);
    entry(settings.enable_buffer_history, false);

    // Graphics > Extras
    entry(settings.skip_cpu_inner_invalidation, false);
    entry(settings.async_presentation, true);
    entry(settings.fix_bloom_effects, false);
    entry(settings.use_asynchronous_shaders, true);
    entry(settings.gpu_unswizzle_texture_size, GpuUnswizzleSize::Large);
    entry(settings.gpu_unswizzle_stream_size, GpuUnswizzle::Medium);
    entry(settings.gpu_unswizzle_chunk_size, GpuUnswizzleChunk::Medium);
    entry(settings.gpu_unswizzle_enabled, false);
#if defined(_WIN32) || defined(__linux__)
    // Other hosts skip the pinning.
    entry(settings.pin_gpu_threads, true);
#endif
#ifndef __APPLE__
    // Extended dynamic state stays disabled on macOS: MoltenVK renders black screens with it.
    entry(settings.dyna_state, ExtendedDynamicState::EDS2);
#endif
    entry(settings.sample_shading, 0);
    entry(settings.vertex_input_dynamic_state, true);

    // System
    entry(settings.cpu_clock, CpuClock::Normal);
    entry(settings.gpu_clock, handheld ? GpuClock::Normal : GpuClock::Boost);
    if (handheld) {
        entry(settings.use_docked_mode, ConsoleMode::Handheld);
        entry(settings.resolution_setup, ResolutionSetup::Res1X);
        entry(settings.anti_aliasing, AntiAliasing::None);
        entry(settings.scaling_filter, ScalingFilter::Bilinear);
        entry(settings.oot_anti_aliasing, OotAntiAliasing::NoSharpening);
        // The game's clock follows the frame rate limit: at 120 (set for a 120 Hz panel or a
        // 60 fps mod) the game ran at double speed on a Legion Go S without the mod.
        entry(settings.frame_rate_limit, u16{60});
    }
    entry(settings.use_multi_core, true);
    entry(settings.memory_layout_mode, MemoryLayout::Memory_4Gb);
    entry(settings.use_speed_limit, true);
    entry(settings.speed_limit, 100);
    entry(settings.slow_speed_limit, 50);
    entry(settings.turbo_speed_limit, 200);
    entry(settings.sync_core_speed, false);

    // CPU
    entry(settings.cpu_accuracy, CpuAccuracy::Auto);
    entry(settings.use_custom_cpu_ticks, false);
}

/// A setting that changed, with its values as BasicSetting::ToString gives them. game names the
/// per-game config (user/config/custom/<game>.ini) whose value changed; it is empty for a global
/// value.
struct RecommendedSettingChange {
    const BasicSetting* setting;
    std::string previous;
    std::string current;
    std::string game;
};

/// Writes the recommended values to the global settings and returns the ones that changed.
/// The in-memory per-game values, and whether a setting uses its per-game value, stay as they
/// are; FrontendCommon::ApplyRecommendedSettings resets the per-game configs on disk.
/// recommended_settings_backup takes the previous value of every recommended setting it does not
/// hold yet. A value it holds stays until it is restored, so applying twice does not lose the
/// values from before the first time.
std::vector<RecommendedSettingChange> ApplyRecommendedSettings(Values& settings);

/// Puts back the global values recommended_settings_backup holds, clears it and returns the
/// settings that changed. Only settings in the recommended table are restored.
std::vector<RecommendedSettingChange> RestorePreviousSettings(Values& settings);

/// Parses a backup of "label:value,label:value", with values as BasicSetting::ToString gives
/// them. Entries without a value are dropped.
std::map<std::string, std::string> ParseRecommendedSettingsBackup(const std::string& backup);

/// Writes values back in the form ParseRecommendedSettingsBackup reads.
std::string FormatRecommendedSettingsBackup(const std::map<std::string, std::string>& entries);

} // namespace Settings
