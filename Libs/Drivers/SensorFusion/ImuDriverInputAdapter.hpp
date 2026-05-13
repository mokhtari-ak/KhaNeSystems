#pragma once

#include <cstdint>

#include "Libs/Drivers/Sensors/Imu/IImuDriver.hpp"

namespace Drivers::SensorFusion {

struct ImuFusionInput {
    std::uint64_t timestamp_us = 0;
    float accel_x = 0.0f;
    float accel_y = 0.0f;
    float accel_z = 0.0f;
    float gyro_x = 0.0f;
    float gyro_y = 0.0f;
    float gyro_z = 0.0f;
    bool valid = false;
};

class ImuDriverInputAdapter {
public:
    explicit ImuDriverInputAdapter(Sensors::Imu::IImuDriver& imu_driver) noexcept : imu_driver_(imu_driver) {}

    ImuFusionInput pull(std::uint64_t timestamp_us) noexcept {
        const auto sample = imu_driver_.readSample(timestamp_us);
        return toFusionInput(sample);
    }

private:
    static ImuFusionInput toFusionInput(const Sensors::Imu::ImuSample& sample) noexcept {
        return {
            .timestamp_us = sample.timestamp_us,
            .accel_x = static_cast<float>(sample.accel_raw.x),
            .accel_y = static_cast<float>(sample.accel_raw.y),
            .accel_z = static_cast<float>(sample.accel_raw.z),
            .gyro_x = static_cast<float>(sample.gyro_raw.x),
            .gyro_y = static_cast<float>(sample.gyro_raw.y),
            .gyro_z = static_cast<float>(sample.gyro_raw.z),
            .valid = sample.status.ok,
        };
    }

    Sensors::Imu::IImuDriver& imu_driver_;
};

} // namespace Drivers::SensorFusion
