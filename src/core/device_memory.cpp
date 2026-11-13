// SPDX-License-Identifier: GPL-2.0-or-later

#if defined(_WIN32) || defined(__linux__)
#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <cstdlib>
#include <mutex>
#include <new>

#include "common/logging.h"
#include "common/memory_detect.h"
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
    // (LaküenPlay) Off by default on this base: with it, v1.0 froze a whole Steam Deck, which the
    // same pass on the build-130 base did not (the new base hands guest memory to the GPU and hints
    // it cold itself, #1581, #1528). EDEN_NX2_ZERO_TRIM=1 punches the idle all-zero pages out of
    // the memfd again (HostMemory::TrimZeroPages)
    const char* zero_trim = std::getenv("EDEN_NX2_ZERO_TRIM");
    if (zero_trim != nullptr && zero_trim[0] == '1') {
        zero_trim_thread = std::jthread{[this](std::stop_token stop) { TrimZeroPages(stop); }};
    }
#endif
}

DeviceMemory::~DeviceMemory() = default;

#if defined(_WIN32) || defined(__linux__)
// Windows can't punch holes in the guest memory section, so pages holding only zeros (games clear
// their heaps while booting: Ocarina leaves ~7 GB) stay resident until they leave the working set.
// (LaküenPlay) Linux punches the idle ones out of the memfd and hands the emulator's freed heap
// back to the system (malloc_trim) while passes still free memory or the host is short of RAM.
// A pass comes every 5 s while they keep turning up, backing off to once every 5 minutes: after
// boot most passes trim 1-2 MiB for about a second of work. It waits out bulk mapping (loads,
// spawns), whose map calls stalled 10-14 ms while a pass ran.
void DeviceMemory::TrimZeroPages(std::stop_token stop) {
    using namespace std::chrono_literals;
    Common::SetCurrentThreadName("ZeroTrim");
    Common::SetCurrentThreadPriority(Common::ThreadPriority::Low);
    constexpr size_t Busy = size_t{64} << 20;
    std::mutex mutex;
    std::condition_variable_any wake;
    std::chrono::seconds interval = 5s;
    // (LaküenPlay) An area change fills memory fast (the kernel zero-fills new pages as the game
    // touches them): when 256 MiB more were filled since the last pass, or the host runs short of
    // RAM, the next pass comes after 5 s instead of after the backed-off interval (up to 5
    // minutes). EDEN_NX2_ZERO_TRIM_AREA=0: wait the interval as before.
    static const bool area_wake = [] {
        const char* value = std::getenv("EDEN_NX2_ZERO_TRIM_AREA");
        return value == nullptr || value[0] != '0';
    }();
    constexpr u64 AreaFillBytes = u64{256} << 20;
    u64 filled_at_pass = Common::HostMemory::ZeroFilledBytes();
    while (true) {
        for (auto waited = std::chrono::seconds{0}; waited < interval; waited += 5s) {
            {
                std::unique_lock lock{mutex};
                wake.wait_for(lock, stop, (std::min)(std::chrono::seconds{5}, interval - waited),
                              [] { return false; });
            }
            if (stop.stop_requested()) {
                return;
            }
            if (area_wake &&
                (Common::HostMemory::ZeroFilledBytes() - filled_at_pass >= AreaFillBytes ||
                 Common::IsHostLowOnMemory())) {
                break;
            }
        }
        if (stop.stop_requested()) {
            return;
        }
        filled_at_pass = Common::HostMemory::ZeroFilledBytes();
        // Busy: over 32 MiB mapped, unmapped or protected in the last half second. At most 10
        // waits (5 s), then the pass runs anyway.
        for (int wait = 0; wait < 10; ++wait) {
            const u64 before = Common::HostMemory::MappedBytes();
            {
                std::unique_lock lock{mutex};
                wake.wait_for(lock, stop, 500ms, [] { return false; });
            }
            if (stop.stop_requested()) {
                return;
            }
            if (Common::HostMemory::MappedBytes() - before < (u64{32} << 20)) {
                break;
            }
        }
        const auto start = std::chrono::steady_clock::now();
        std::pair<size_t, size_t> result;
        try {
            result = buffer.TrimZeroPages();
        } catch (const std::bad_alloc&) {
            // A pass needs 32 MiB and more of scratch. When Windows runs out of commit, the
            // exception leaving this thread ended the emulator (#1346); the pass is only a saving.
            interval = std::chrono::seconds{60};
            const Common::ProcessMemoryUsage usage = Common::GetProcessMemoryUsage();
            LOG_WARNING(HW_Memory,
                        "Skipped trimming all-zero guest pages: out of memory. Windows commit is "
                        "{} of {} MiB (this process {} MiB); the limit is RAM plus the page file",
                        usage.system_commit >> 20, usage.system_commit_limit >> 20,
                        usage.commit >> 20);
            continue;
        }
        const auto [resident, trimmed] = result;
#if defined(__linux__) && defined(__GLIBC__)
        // malloc_trim walks and locks every heap arena, so a thread that allocates meanwhile
        // waits; a Steam Deck showed a ~300-390 ms frame every 45-70 s, the trim pass period. It
        // runs only while the passes still free memory (boot, area changes) or the host is short
        // of RAM. EDEN_NX2_MALLOC_TRIM=1 runs it on every pass.
        static const bool always_malloc_trim = [] {
            const char* value = std::getenv("EDEN_NX2_MALLOC_TRIM");
            return value != nullptr && value[0] == '1';
        }();
        if (always_malloc_trim || trimmed >= Busy || Common::IsHostLowOnMemory()) {
            malloc_trim(0);
        }
#endif
        const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                            std::chrono::steady_clock::now() - start)
                            .count();
        if (trimmed < Busy) {
            interval = (std::min)(interval * 2, std::chrono::seconds{300});
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
