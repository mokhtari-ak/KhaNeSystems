#include <gtest/gtest.h>
#include "khane_systems/Propulsion/FlightController.hpp"

TEST(FlightControllerTest, QuadcopterSelectionWorks) {
    control::FlightController<control::QuadcopterDynamics> controller;
    
    // Check that we are using QuadcopterDynamics
    static_assert(!control::QuadcopterDynamics::IS_FIXED_WING, "Should be quadcopter");
    
    // If it compiles, the selection is correct.
    controller.update(0.5f, 0.0f, 0.0f, 0.0f);
    SUCCEED();
}

TEST(FlightControllerTest, FixedWingSelectionWorks) {
    control::FlightController<control::FixedWingDynamics> controller;
    
    static_assert(control::FixedWingDynamics::IS_FIXED_WING, "Should be fixed-wing");
    
    controller.update(0.5f, 0.0f, 0.0f, 0.0f);
    SUCCEED();
}
