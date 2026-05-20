#include <gtest/gtest.h>
#include "khane_systems/supervisor/HeartbeatMonitor.hpp"

TEST(HeartbeatMonitorTest, HealthCheckWorks) {
    khane::supervisor::HeartbeatMonitor monitor;
    monitor.register_module(1, 100);

    monitor.process_heartbeat(1, 10);
    monitor.check_system_health(50);
    EXPECT_TRUE(monitor.is_system_healthy());

    // Trigger timeout
    monitor.check_system_health(150);
    EXPECT_FALSE(monitor.is_system_healthy());
}
