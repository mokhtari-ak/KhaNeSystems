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

TEST(SensorFusionTest, IgnoresInvalidImuSamples) {
    khane::sensor_fusion::SensorFusion sf;
    const float invalid_imu[6] = {0.0f, 0.0f, 30.0f, 1.0f, 2.0f, 3.0f};

    sf.update_sensors(invalid_imu, nullptr);
    auto pos = sf.get_position();

    EXPECT_FLOAT_EQ(pos.vx, 0.0f);
    EXPECT_FLOAT_EQ(pos.vy, 0.0f);
    EXPECT_FLOAT_EQ(pos.vz, 0.0f);
}

TEST(SensorFusionTest, UpdatesVelocityForNominalImuSamples) {
    khane::sensor_fusion::SensorFusion sf;
    const float imu[6] = {0.0f, 0.0f, 9.81f, 1.0f, 2.0f, 3.0f};

    sf.update_sensors(imu, nullptr);
    auto pos = sf.get_position();

    EXPECT_FLOAT_EQ(pos.vx, 1.0f);
    EXPECT_FLOAT_EQ(pos.vy, 2.0f);
    EXPECT_FLOAT_EQ(pos.vz, 3.0f);
}
