#include <gtest/gtest.h>
#include "khane_systems/sensor_fusion/SensorFusion.hpp"

TEST(SensorFusionTest, ProvidesValidAttitude) {
    khane::sensor_fusion::SensorFusion sf;
    auto att = sf.get_attitude();
    
    EXPECT_FLOAT_EQ(att.qw, 1.0f);
    EXPECT_FLOAT_EQ(att.roll, 0.0f);
}

TEST(SensorFusionTest, ProvidesValidPosition) {
    khane::sensor_fusion::SensorFusion sf;
    auto pos = sf.get_position();
    
    EXPECT_FLOAT_EQ(pos.x, 0.0f);
    EXPECT_FLOAT_EQ(pos.z, 0.0f);
}
