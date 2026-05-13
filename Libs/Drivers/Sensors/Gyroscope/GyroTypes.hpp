#pragma once

#include <cstdint>

#include "Libs/Drivers/Sensors/Common/DriverStatus.hpp"

namespace Drivers::Sensors::Gyroscope {

struct GyroSample {
    std::uint64_t timestamp_us = 0;
    std::int16_t x = 0;
    std::int16_t y = 0;
    std::int16_t z = 0;
    Common::DriverStatus status{.ok = false, .error = Common::DriverError::NotInitialized};
};

struct GyroConfig {
    std::uint8_t i2c_address = 0x68;
    std::uint8_t data_start_register = 0x43; // typical GYRO_XOUT_H
};

} // namespace Drivers::Sensors::Gyroscope
