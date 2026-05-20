#include <gtest/gtest.h>
#include "khane_systems/sensor_fusion/SensorFusion.hpp"
#include <limits>

TEST(SensorFusionTest, NaNValidationWorks) {
    khane::sensor_fusion::SensorFusion sf;
    float invalid_data[] = { std::numeric_limits<float>::quiet_NaN() };
    
    // update_sensors should internally handle this invalid data
    sf.update_sensors(invalid_data, nullptr);
    
    // Verify attitude remains default (no update)
    auto att = sf.get_attitude();
    EXPECT_FLOAT_EQ(att.qw, 1.0f);
}
