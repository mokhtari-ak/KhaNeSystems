#pragma once

#include <functional>

#include "AirSpeedTypes.hpp"

namespace Drivers::Sensors::AirSpeed {

class IAirSpeedDriver {
public:
    using SampleCallback = std::function<void(const AirSpeedSample&)>;

    virtual ~IAirSpeedDriver() = default;

    virtual Common::DriverStatus init() noexcept = 0;
    virtual AirSpeedSample readSample(std::uint64_t timestamp_us) noexcept = 0;
    virtual void setSampleCallback(SampleCallback callback) = 0;
    virtual Common::DriverStatus pollAndPublish(std::uint64_t timestamp_us) noexcept = 0;
};

} // namespace Drivers::Sensors::AirSpeed
