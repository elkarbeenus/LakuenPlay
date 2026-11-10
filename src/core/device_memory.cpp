// SPDX-FileCopyrightText: Copyright 2020 yuzu Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#if defined(_WIN32) || defined(__linux__)
#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <cstdlib>
#include <mutex>

#include "common/logging.h"
#include "common/thread.h"
#endif
#if defined(__linux__) && defined(__GLIBC__)
#include <malloc.h>
#endif

#include "core/device_memory.h"
#include "hle/kernel/board/nintendo/nx/k_system_control.h"

namespace Core {

#ifdef HAS_NCE
constexpr size_t VirtualReserveSize = 1ULL << 38;
#else
constexpr size_t VirtualReserveSize = 1ULL << 39;
#endif

DeviceMemory::DeviceMemory()
    : buffer{Kernel::Board::Nintendo::Nx::KSystemControl::Init::GetIntendedMemorySize(),
             VirtualReserveSize} {
#if defined(_WIN32)
    zero_trim_thread = std::jthread{[this](std::stop_token stop) { TrimZeroPages(stop); }};
#elif defined(__linux__)
    // EDEN_NX2_ZERO_TRIM=0 turns it off on Linux
    const char* zero_trim = std::getenv("EDEN_NX2_ZERO_TRIM");
    if (zero_trim == nullptr || zero_trim[0] != '0') {
        zero_trim_thread = std::jthread{[this](std::stop_token stop) { TrimZeroPages(stop); }};
    }
#endif
}

DeviceMemory::~DeviceMemory() = default;

#if defined(_WIN32) || defined(__linux__)
// Pages holding only zeros (games clear their heaps while booting: Ocarina leaves ~7 GB) stay
// resident: Windows takes them out of the working set, Linux punches the idle ones out of the
// memfd (HostMemory::TrimZeroPages). On Linux each pass also hands the emulator's freed heap
// memory back to the system (malloc_trim). A pass comes every 5 s while they keep turning up,
// backing off to once a minute.
void DeviceMemory::TrimZeroPages(std::stop_token stop) {
    using namespace std::chrono_literals;
    Common::SetCurrentThreadName("ZeroTrim");
    Common::SetCurrentThreadPriority(Common::ThreadPriority::Low);
    constexpr size_t Busy = size_t{64} << 20;
    std::mutex mutex;
    std::condition_variable_any wake;
    std::chrono::seconds interval = 5s;
    while (true) {
        {
            std::unique_lock lock{mutex};
            wake.wait_for(lock, stop, interval, [] { return false; });
        }
        if (stop.stop_requested()) {
            return;
        }
        const auto start = std::chrono::steady_clock::now();
        const auto [resident, trimmed] = buffer.TrimZeroPages();
#if defined(__linux__) && defined(__GLIBC__)
        // malloc_trim walks and locks every heap arena, so a thread that allocates meanwhile
        // waits; a Steam Deck showed a ~300-390 ms frame every 45-70 s, the trim pass period.
        // The handheld build runs it only while the zero-page passes still free memory (boot,
        // area changes), not on the idle passes. EDEN_NX2_MALLOC_TRIM=1 runs it on every pass.
        static const bool always_malloc_trim = [] {
            const char* value = std::getenv("EDEN_NX2_MALLOC_TRIM");
            return value != nullptr && value[0] == '1';
        }();
        if (always_malloc_trim || trimmed >= Busy) {
            malloc_trim(0);
        }
#endif
        const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                            std::chrono::steady_clock::now() - start)
                            .count();
        if (trimmed < Busy) {
            interval = (std::min)(interval * 2, std::chrono::seconds{60});
            continue;
        }
        interval = 5s;
        LOG_INFO(HW_Memory,
                 "Trimmed {} MiB of all-zero guest pages out of {} MiB resident in {} ms; the kernel "
                 "zero-filled {} MiB so far",
                 trimmed >> 20, resident >> 20, ms, Common::HostMemory::ZeroFilledBytes() >> 20);
    }
}
#endif

} // namespace Core
