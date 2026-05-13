#pragma once

#include <cstdint>

namespace Drivers::Sensors::Common {

enum class DriverError : std::uint8_t {
    None = 0,
    NotInitialized,
    TransportError,
    Timeout,
    InvalidData,
    DeviceNotFound
};

struct DriverStatus {
    bool ok = false;
    DriverError error = DriverError::None;
};

} // namespace Drivers::Sensors::Common
