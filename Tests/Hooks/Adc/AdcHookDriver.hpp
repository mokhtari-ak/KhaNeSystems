#pragma once

#include <cstdint>
#include <deque>
#include <optional>
#include <unordered_map>

namespace Tests::Hooks::Adc {

enum class AdcHookFault {
    None,
    Timeout,
    InvalidSample
};

struct AdcReadResult {
    bool success = false;
    std::uint16_t sample = 0;
    AdcHookFault fault = AdcHookFault::None;
};

class AdcHookDriver {
public:
    void init() noexcept { initialized_ = true; }

    void pushSample(std::uint8_t channel, std::uint16_t sample) {
        scripted_samples_[channel].push_back(sample);
    }

    void setFault(std::uint8_t channel, AdcHookFault fault) { channel_faults_[channel] = fault; }

    void clearFault(std::uint8_t channel) { channel_faults_.erase(channel); }

    AdcReadResult read(std::uint8_t channel) {
        if (!initialized_) {
            return {.success = false, .sample = 0, .fault = AdcHookFault::Timeout};
        }

        const auto fault_it = channel_faults_.find(channel);
        if (fault_it != channel_faults_.end() && fault_it->second != AdcHookFault::None) {
            return {.success = false, .sample = 0, .fault = fault_it->second};
        }

        auto sample_it = scripted_samples_.find(channel);
        if (sample_it == scripted_samples_.end() || sample_it->second.empty()) {
            return {.success = false, .sample = 0, .fault = AdcHookFault::InvalidSample};
        }

        const std::uint16_t sample = sample_it->second.front();
        sample_it->second.pop_front();
        return {.success = true, .sample = sample, .fault = AdcHookFault::None};
    }

private:
    bool initialized_ = false;
    std::unordered_map<std::uint8_t, std::deque<std::uint16_t>> scripted_samples_{};
    std::unordered_map<std::uint8_t, AdcHookFault> channel_faults_{};
};

} // namespace Tests::Hooks::Adc
