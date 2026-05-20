#include <gtest/gtest.h>
#include "khane_systems/supervisor/CpuThrottler.hpp"

TEST(CpuThrottlerTest, ModeSwitchingWorks) {
    khane::supervisor::CpuThrottler throttler(0.85f);

    throttler.update_load(0.5f);
    EXPECT_EQ(throttler.get_mode(), khane::supervisor::PerformanceMode::NOMINAL);

    throttler.update_load(0.9f);
    EXPECT_EQ(throttler.get_mode(), khane::supervisor::PerformanceMode::THROTTLED);

    throttler.update_load(0.1f);
    EXPECT_EQ(throttler.get_mode(), khane::supervisor::PerformanceMode::NOMINAL);
}
