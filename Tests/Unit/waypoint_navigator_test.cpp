#include <gtest/gtest.h>
#include "khane_systems/navigation/WaypointNavigator.hpp"

TEST(WaypointNavigatorTest, AdvancesThroughWaypoints) {
    khane::navigation::WaypointNavigator nav;
    nav.add_waypoint({1.0f, 1.0f, 10.0f});
    nav.add_waypoint({2.0f, 2.0f, 10.0f});

    // At first waypoint
    nav.update(1.0f, 1.0f, 10.0f);
    EXPECT_FALSE(nav.is_mission_complete());

    // At second waypoint
    nav.update(2.0f, 2.0f, 10.0f);
    EXPECT_TRUE(nav.is_mission_complete());
}
