#pragma once

#include <cstdint>

namespace khane::supervisor {

enum class PerformanceMode {
    NOMINAL,
    THROTTLED
};

class CpuThrottler {
public:
    CpuThrottler(float threshold = 0.85f);

    // Update current CPU load (0.0 to 1.0)
    void update_load(float load);

    // Get current mode
    PerformanceMode get_mode() const { return current_mode_; }

private:
    float threshold_;
    PerformanceMode current_mode_;
};

} // namespace khane::supervisor
