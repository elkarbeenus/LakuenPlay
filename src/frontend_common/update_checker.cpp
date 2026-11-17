// SPDX-FileCopyrightText: Copyright 2026 Eden Emulator Project
// SPDX-License-Identifier: GPL-3.0-or-later

// Copyright Citra Emulator Project / Azahar Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <algorithm>
#include <charconv>
#include <string>
#include <vector>
#include "common/logging.h"
#include "common/scm_rev.h"
#include "frontend_common/update_checker.h"

namespace UpdateChecker {

namespace {

// (LaküenPlay) A release tag as this fork writes them: a track ("" for the PC's v1.2, "handheld-" for handheld-v1.0.5,
// "build-" for Lichii's own build-N) and its numbers, compared in order. A tag of another form is no release to update to.
struct Version {
    std::string track;
    std::vector<u64> numbers;
};

std::optional<Version> ParseVersion(std::string_view tag) {
    Version v;
    if (tag.starts_with("build-")) {
        v.track = "build-";
        tag.remove_prefix(6);
    } else {
        if (tag.starts_with("handheld-")) {
            v.track = "handheld-";
            tag.remove_prefix(9);
        }
        if (!tag.starts_with('v')) {
            return std::nullopt;
        }
        tag.remove_prefix(1);
    }
    while (!tag.empty()) {
        u64 number{};
        const auto [end, error] = std::from_chars(tag.data(), tag.data() + tag.size(), number);
        if (error != std::errc{} || end == tag.data()) {
            return std::nullopt;
        }
        v.numbers.push_back(number);
        tag.remove_prefix(static_cast<size_t>(end - tag.data()));
        if (tag.empty()) {
            break;
        }
        if (tag.front() != '.') {
            return std::nullopt;
        }
        tag.remove_prefix(1);
    }
    if (v.numbers.empty() || (v.track == "build-" && v.numbers.front() == 0)) {
        return std::nullopt;
    }
    return v;
}

bool Newer(const Version& a, const Version& b) {   // (a newer than b: 1.0.10 after 1.0.9, 1.1 after 1.0.5)
    const size_t n = std::max(a.numbers.size(), b.numbers.size());
    for (size_t i = 0; i < n; i++) {
        const u64 x = i < a.numbers.size() ? a.numbers[i] : 0, y = i < b.numbers.size() ? b.numbers[i] : 0;
        if (x != y) {
            return x > y;
        }
    }
    return false;
}

} // namespace

Failure ClassifyFailure(int status) {
    if (status == 403 || status == 429) {
        return Failure::RateLimited;
    }
    return status >= 400 ? Failure::Http : Failure::Network;
}

std::optional<Common::Net::Release> SelectUpdate(std::span<const Common::Net::Release> releases,
                                                 std::string_view build_tag,
                                                 std::string_view platform,
                                                 Common::Net::LinuxPackage package) {
    auto newest = ParseVersion(build_tag);
    if (!newest) {
        return std::nullopt;
    }

    const Common::Net::Release* selected = nullptr;
    for (const auto& release : releases) {
        const auto version = ParseVersion(release.tag);
        if (version && version->track == newest->track && Newer(*version, *newest) &&
            !release.GetPlatformAssets(platform, package).empty()) {
            newest = version;
            selected = &release;
        }
    }
    return selected ? std::make_optional(*selected) : std::nullopt;
}

bool IsReleaseBuild(std::string_view build_tag, std::string_view platform) {
    // (LaküenPlay) this fork's tags: v1.3.2, handheld-v1.0.8 or build-N (ParseVersion)
    return ParseVersion(build_tag).has_value() && !platform.empty();
}

CheckResult Check() {
    if (!ParseVersion(Common::g_build_release_tag) ||
        std::string_view{Common::g_build_release_platform}.empty()) {
        return {.failure = Failure::NotReleaseBuild};
    }

    const auto list = Common::Net::GetReleases();
    if (!list.ok) {
        const auto failure = ClassifyFailure(list.status);
        LOG_WARNING(Frontend, "Update check failed with status {}", list.status);
        return {.failure = failure, .status = list.status};
    }

    auto update = SelectUpdate(list.releases, Common::g_build_release_tag,
                               Common::g_build_release_platform,
                               Common::Net::CurrentLinuxPackage());
    if (update) {
        LOG_INFO(Frontend, "Received update {}", update->title);
    }
    return {.update = std::move(update)};
}

std::optional<Common::Net::Release> GetUpdate() {
    return Check().update;
}

} // namespace UpdateChecker
