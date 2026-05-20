#include <gtest/gtest.h>
#include "khane_systems/supervisor/FailsafeManager.hpp"

TEST(FailsafeManagerTest, SeverityEscalationWorks) {
    khane::supervisor::FailsafeManager manager;

    manager.trigger_failsafe(khane::supervisor::FailsafeLevel::WARNING);
    EXPECT_EQ(manager.get_current_level(), khane::supervisor::FailsafeLevel::WARNING);

    // Escalation
    manager.trigger_failsafe(khane::supervisor::FailsafeLevel::CRITICAL);
    EXPECT_EQ(manager.get_current_level(), khane::supervisor::FailsafeLevel::CRITICAL);

    // Should not downgrade
    manager.trigger_failsafe(khane::supervisor::FailsafeLevel::WARNING);
    EXPECT_EQ(manager.get_current_level(), khane::supervisor::FailsafeLevel::CRITICAL);
}
