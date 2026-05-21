#include "CpuThrottler.hpp"

namespace khane::supervisor {

CpuThrottler::CpuThrottler(float threshold) 
    : threshold_(threshold), current_mode_(PerformanceMode::NOMINAL) {}

void CpuThrottler::update_load(float load) {
    if (load > threshold_) {
        current_mode_ = PerformanceMode::THROTTLED;
    } else {
        current_mode_ = PerformanceMode::NOMINAL;
    }
}

} // namespace khane::supervisor
