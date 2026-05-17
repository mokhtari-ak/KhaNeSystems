#pragma once
#include "Common/SensorTypes.hpp"

namespace Drivers {

    class GNSSDriver : public Sensors::SensorInterface<Sensors::GNSSData> {
    public:
        GNSSDriver() noexcept = default;
        Sensors::GNSSData update() noexcept override;
        bool calibrate() noexcept override { return true; }
    };

    class BaroDriver : public Sensors::SensorInterface<Sensors::BaroData> {
    public:
        BaroDriver() noexcept = default;
        Sensors::BaroData update() noexcept override;
        bool calibrate() noexcept override;
    };

    class AirSpeedDriver : public Sensors::SensorInterface<Sensors::AirSpeedData> {
    public:
        AirSpeedDriver() noexcept = default;
        Sensors::AirSpeedData update() noexcept override;
        bool calibrate() noexcept override;
    };

} // namespace Drivers
