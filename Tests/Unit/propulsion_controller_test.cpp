#include <gtest/gtest.h>
#include "khane_systems/propulsion/propulsion_controller.hpp"

// Mock class to simulate the state machine
namespace autopilot {
    class state_machine {
    public:
        bool allowed = false;
        bool is_propulsion_allowed() const noexcept { return allowed; }
    };
}

TEST(PropulsionControllerTest, SafetyLockForcesZeroThrust) {
    autopilot::state_machine sm;
    control::propulsion_controller pc(sm);

    sm.allowed = false;
    bool success = pc.apply_thrust(0.5f, 12.0f);
    
    EXPECT_FALSE(success);
    EXPECT_EQ(pc.get_current_thrust(), 0.0f);

    sm.allowed = true;
    success = pc.apply_thrust(0.8f, 12.0f);
    EXPECT_TRUE(success);
    EXPECT_EQ(pc.get_current_thrust(), 0.8f);
}

TEST(PropulsionControllerTest, VoltageSagLimitsThrust) {
    autopilot::state_machine sm;
    control::propulsion_controller pc(sm);
    sm.allowed = true;

    // Normal voltage
    pc.apply_thrust(0.8f, 12.0f);
    EXPECT_EQ(pc.get_current_thrust(), 0.8f);

    // Sagging voltage, should limit to 0.5f
    pc.apply_thrust(0.8f, 9.0f);
    EXPECT_EQ(pc.get_current_thrust(), 0.5f);
}
