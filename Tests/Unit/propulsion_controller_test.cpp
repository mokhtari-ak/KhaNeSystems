#include <gtest/gtest.h>
#include "khane_systems/Propulsion/propulsion_controller.hpp"
#include "khane_systems/Autopilot/state_machine.hpp"

TEST(PropulsionControllerTest, SafetyLockForcesZeroThrust) {
    autopilot::state_machine sm;
    control::propulsion_controller pc(sm);

    bool success = pc.apply_thrust(0.5f, 12.0f);
    
    EXPECT_FALSE(success);
    EXPECT_EQ(pc.get_current_thrust(), 0.0f);

    sm.transition_to(autopilot::flight_state::armed);
    success = pc.apply_thrust(0.8f, 12.0f);
    EXPECT_TRUE(success);
    EXPECT_EQ(pc.get_current_thrust(), 0.8f);
}

TEST(PropulsionControllerTest, VoltageSagLimitsThrust) {
    autopilot::state_machine sm;
    control::propulsion_controller pc(sm);
    sm.transition_to(autopilot::flight_state::armed);

    // Normal voltage
    pc.apply_thrust(0.8f, 12.0f);
    EXPECT_EQ(pc.get_current_thrust(), 0.8f);

    // Sagging voltage, should limit to 0.5f
    pc.apply_thrust(0.8f, 9.0f);
    EXPECT_EQ(pc.get_current_thrust(), 0.5f);
}

TEST(PropulsionControllerTest, ClampsThrustToValidRange) {
    autopilot::state_machine sm;
    control::propulsion_controller pc(sm);
    sm.transition_to(autopilot::flight_state::armed);

    pc.apply_thrust(1.5f, 12.0f);
    EXPECT_EQ(pc.get_current_thrust(), 1.0f);

    pc.apply_thrust(-0.2f, 12.0f);
    EXPECT_EQ(pc.get_current_thrust(), 0.0f);
}
