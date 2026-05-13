#pragma once

#include <cstdint>

#include "Libs/Drivers/Sensors/Common/DriverStatus.hpp"

namespace Drivers::Sensors::AirSpeed {

struct AirSpeedSample {
    std::uint64_t timestamp_us = 0;
    std::int16_t differential_pressure_raw = 0;
    Common::DriverStatus status{.ok = false, .error = Common::DriverError::NotInitialized};
};

struct AirSpeedConfig {
    std::uint8_t i2c_address = 0x28;
    std::uint8_t read_command = 0x00;
};

} // namespace Drivers::Sensors::AirSpeed
