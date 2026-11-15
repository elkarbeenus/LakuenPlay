// SPDX-FileCopyrightText: Copyright 2026 Eden Emulator Project
// SPDX-License-Identifier: GPL-3.0-or-later

// SPDX-FileCopyrightText: 2014 Citra Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

namespace Common {

extern const char g_scm_rev[];
extern const char g_scm_branch[];
extern const char g_scm_desc[];
extern const char g_build_name[];
extern const char g_build_date[];
extern const char g_build_fullname[];
extern const char g_build_version[];
extern const char g_build_id[];
extern const char g_build_release_tag[];
extern const char g_build_release_platform[];
// (LaküenPlay) PC, Handheld or LegionGoS: the device in the release assets' names
extern const char g_lakuen_asset_device[];
extern const char g_title_bar_format_idle[];
extern const char g_title_bar_format_running[];
extern const char g_shader_cache_version[];
extern const char g_compiler_id[];

// (LaküenPlay) The name in the window title, the log and the About dialog. It is based on Lichii
// and Eden; data folders and settings stay those of Lichii.
// (LaküenPlay) the build's device, from CMake's LAKUEN_ASSET_DEVICE (one build per device)
#if defined(LAKUEN_DEVICE_PC)
inline constexpr const char g_app_label[] = "LaküenPlay – PC";
#elif defined(LAKUEN_DEVICE_LEGION_GO_S)
inline constexpr const char g_app_label[] = "LaküenPlay – Legion Go S";
#else
inline constexpr const char g_app_label[] = "LaküenPlay – Handheld";
#endif

extern const bool g_is_dev_build;
extern const bool g_is_nightly_build;

extern const char g_build_auto_update_website[];
extern const char g_build_auto_update_api_path[];
extern const char g_build_auto_update_repo[];
extern const char g_build_auto_update_stable_api[];
extern const char g_build_auto_update_stable_repo[];

} // namespace Common
