// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "common/settings.h"

namespace Settings {

inline constexpr u64 MarioKartWorldProgramId = 0x0400C3F00006E000ULL;

inline void SetMarioKartWorldRecommendedSettings(Values& settings, bool global) {
    const auto session = [global](auto& setting, auto value) {
        setting.SetGlobal(global);
        setting.SetValue(value);
    };
    session(settings.renderer_backend, RendererBackend::Vulkan);
    session(settings.resolution_setup, ResolutionSetup::Res1X);
    session(settings.gpu_accuracy, GpuAccuracy::Low);
    session(settings.use_asynchronous_shaders, true);
    session(settings.astc_recompression, AstcRecompression::Bc3);
    session(settings.vram_usage_mode, VramUsageMode::Conservative);
    session(settings.use_reactive_flushing, false);
    session(settings.vsync_mode, VSyncMode::FifoRelaxed);
    session(settings.use_docked_mode, ConsoleMode::Handheld);
    session(settings.scaling_filter, ScalingFilter::ScaleForce);
    session(settings.anti_aliasing, AntiAliasing::Fxaa);
}

inline bool ApplyMarioKartWorldPreset(Values& settings, u64 program_id, bool accepted) {
    if (!accepted || program_id != MarioKartWorldProgramId) {
        return false;
    }
    SetMarioKartWorldRecommendedSettings(settings, false);
    return true;
}

/// (this fork, PC build) Pokémon Pokopia and Ocarina of Time, for the per-game "Recommended
/// settings for this PC" entries of the Help menu.
inline constexpr u64 PokopiaProgramId = 0x0400A3A01EE4C000ULL;
inline constexpr u64 OcarinaProgramId = 0x0400486024DEC000ULL;

/// Pokopia on a desktop PC: docked at 1x (1920x1080), the game's 60 Hz with frame generation x2 for
/// 120 fps on screen without the game running faster, every cached shader built on the loading
/// screen (the first use of an ability built its programs mid-game: a short freeze).
inline void SetPokopiaPcRecommendedSettings(Values& settings) {
    const auto game = [](auto& setting, auto value) {
        setting.SetGlobal(false);
        setting.SetValue(value);
    };
    game(settings.renderer_backend, RendererBackend::Vulkan);
    game(settings.use_docked_mode, ConsoleMode::Docked);
    game(settings.resolution_setup, ResolutionSetup::Res1X);
    game(settings.frame_rate_limit, u16{60});
    game(settings.use_speed_limit, true);
    game(settings.speed_limit, u16{100});
    game(settings.frame_gen, true);
    game(settings.frame_gen_type, FrameGenType::Fsr3);
    game(settings.frame_gen_multiplier, u32{2});
    game(settings.use_asynchronous_shaders, true);
    game(settings.native_async_draws, NativeAsyncDraws::Bounded);
    game(settings.precache_shaders, PrecacheShaders::Always);
}

/// Ocarina of Time on a desktop PC: docked at 1x, 60 Hz at normal speed, every cached shader built on
/// the loading screen.
inline void SetOcarinaPcRecommendedSettings(Values& settings) {
    const auto game = [](auto& setting, auto value) {
        setting.SetGlobal(false);
        setting.SetValue(value);
    };
    game(settings.renderer_backend, RendererBackend::Vulkan);
    game(settings.use_docked_mode, ConsoleMode::Docked);
    game(settings.resolution_setup, ResolutionSetup::Res1X);
    game(settings.frame_rate_limit, u16{60});
    game(settings.use_speed_limit, true);
    game(settings.speed_limit, u16{100});
    game(settings.use_asynchronous_shaders, true);
    game(settings.native_async_draws, NativeAsyncDraws::Bounded);
    game(settings.precache_shaders, PrecacheShaders::Always);
}

} // namespace Settings
