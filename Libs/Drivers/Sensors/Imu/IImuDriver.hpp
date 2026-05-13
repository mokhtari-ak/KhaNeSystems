#pragma once

#include <functional>

#include "ImuTypes.hpp"

namespace Drivers::Sensors::Imu {

class IImuDriver {
public:
    using SampleCallback = std::function<void(const ImuSample&)>;

    virtual ~IImuDriver() = default;

    virtual Common::DriverStatus init() noexcept = 0;
    virtual ImuSample readSample(std::uint64_t timestamp_us) noexcept = 0;
    virtual void setSampleCallback(SampleCallback callback) = 0;
    virtual Common::DriverStatus pollAndPublish(std::uint64_t timestamp_us) noexcept = 0;
};

} // namespace Drivers::Sensors::Imu
