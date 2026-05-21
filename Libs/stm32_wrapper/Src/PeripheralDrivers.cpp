#include "PeripheralDrivers.hpp"

namespace Drivers {

    Sensors::GNSSData GNSSDriver::update() noexcept {
        return Sensors::GNSSData{.status_flags = Sensors::IMUData::FLAG_ERROR};
    }

    Sensors::BaroData BaroDriver::update() noexcept {
        return Sensors::BaroData{.status_flags = Sensors::IMUData::FLAG_ERROR};
    }

    bool BaroDriver::calibrate() noexcept { return true; }

    Sensors::AirSpeedData AirSpeedDriver::update() noexcept {
        return Sensors::AirSpeedData{.status_flags = Sensors::IMUData::FLAG_ERROR};
    }

    bool AirSpeedDriver::calibrate() noexcept { return true; }

} // namespace Drivers
