#pragma once

#include <cstdint>

#include "Libs/Drivers/Sensors/Common/DriverStatus.hpp"

namespace Drivers::Sensors::Imu {

struct Vec3i16 {
    std::int16_t x = 0;
    std::int16_t y = 0;
    std::int16_t z = 0;
};

struct ImuSample {
    std::uint64_t timestamp_us = 0;
    Vec3i16 accel_raw{};
    Vec3i16 gyro_raw{};
    Common::DriverStatus status{.ok = false, .error = Common::DriverError::NotInitialized};
};

struct ImuConfig {
    std::uint8_t i2c_address = 0x68;
    std::uint8_t who_am_i_register = 0x75;
    std::uint8_t expected_who_am_i = 0x68;
    std::uint8_t data_start_register = 0x3B; // ACCEL_XOUT_H style
};

} // namespace Drivers::Sensors::Imu
