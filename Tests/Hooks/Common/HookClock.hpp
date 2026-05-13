#pragma once

#include <cstdint>

namespace Tests::Hooks::Common {

class HookClock {
public:
    static void reset(std::uint64_t start_us = 0) noexcept { now_us_ = start_us; }

    static std::uint64_t nowUs() noexcept { return now_us_; }

    static void advanceUs(std::uint64_t delta_us) noexcept { now_us_ += delta_us; }

    static void sleepUntil(std::uint64_t target_us) noexcept {
        if (target_us > now_us_) {
            now_us_ = target_us;
        }
    }

private:
    inline static std::uint64_t now_us_ = 0;
};

} // namespace Tests::Hooks::Common
