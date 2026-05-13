#pragma once

#include <cstdint>
#include <utility>

#include "IAirSpeedDriver.hpp"

namespace Drivers::Sensors::AirSpeed {

template <typename I2cTransport>
class AirSpeedDriver final : public IAirSpeedDriver {
public:
    AirSpeedDriver(I2cTransport& i2c, AirSpeedConfig config) noexcept : i2c_(i2c), config_(config) {}

    Common::DriverStatus init() noexcept override {
        initialized_ = true;
        return {.ok = true, .error = Common::DriverError::None};
    }

    AirSpeedSample readSample(std::uint64_t timestamp_us) noexcept override {
        AirSpeedSample sample{};
        sample.timestamp_us = timestamp_us;
        if (!initialized_) {
            sample.status = {.ok = false, .error = Common::DriverError::NotInitialized};
            return sample;
        }

        std::uint8_t cmd = config_.read_command;
        std::uint8_t rx[2] = {};
        const auto transfer = i2c_.transfer(config_.i2c_address, &cmd, 1, rx, sizeof(rx));
        if (!transfer.success || transfer.rx_size < sizeof(rx)) {
            sample.status = {.ok = false, .error = Common::DriverError::TransportError};
            return sample;
        }

        sample.differential_pressure_raw =
            static_cast<std::int16_t>((static_cast<std::uint16_t>(rx[0]) << 8) | rx[1]);
        sample.status = {.ok = true, .error = Common::DriverError::None};
        return sample;
    }

    void setSampleCallback(SampleCallback callback) override { callback_ = std::move(callback); }

    Common::DriverStatus pollAndPublish(std::uint64_t timestamp_us) noexcept override {
        const AirSpeedSample sample = readSample(timestamp_us);
        if (callback_) {
            callback_(sample);
        }
        return sample.status;
    }

private:
    I2cTransport& i2c_;
    AirSpeedConfig config_{};
    bool initialized_ = false;
    SampleCallback callback_{};
};

} // namespace Drivers::Sensors::AirSpeed
