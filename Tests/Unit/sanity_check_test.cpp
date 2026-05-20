#include <gtest/gtest.h>
#include "khane_systems/sensor_fusion/SensorFusion.hpp"

TEST(SensorFusionTest, ImplausibleAccelerationRejected) {
    khane::sensor_fusion::SensorFusion sf;
    
    // Acceleration far from 1g (e.g., 20.0f)
    float invalid_imu[] = { 20.0f, 0.0f, 0.0f };
    
    sf.update_sensors(invalid_imu, nullptr);
    
    // Verify attitude remains default (no update)
    auto att = sf.get_attitude();
    EXPECT_FLOAT_EQ(att.qw, 1.0f);
}
