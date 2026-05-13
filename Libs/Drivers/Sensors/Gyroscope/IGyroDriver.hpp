#pragma once

#include <functional>

#include "GyroTypes.hpp"

namespace Drivers::Sensors::Gyroscope {

class IGyroDriver {
public:
    using SampleCallback = std::function<void(const GyroSample&)>;

    virtual ~IGyroDriver() = default;

    virtual Common::DriverStatus init() noexcept = 0;
    virtual GyroSample readSample(std::uint64_t timestamp_us) noexcept = 0;
    virtual void setSampleCallback(SampleCallback callback) = 0;
    virtual Common::DriverStatus pollAndPublish(std::uint64_t timestamp_us) noexcept = 0;
};

} // namespace Drivers::Sensors::Gyroscope
