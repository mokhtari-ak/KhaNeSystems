#pragma once

#include <cstddef>
#include <cstdint>
#include <utility>

#include "IGyroDriver.hpp"

namespace Drivers::Sensors::Gyroscope {

template <typename Transport>
class GyroDriver final : public IGyroDriver {
public:
    GyroDriver(Transport& transport, GyroConfig config) noexcept : transport_(transport), config_(config) {}

    Common::DriverStatus init() noexcept override {
        initialized_ = true;
        return {.ok = true, .error = Common::DriverError::None};
    }

    GyroSample readSample(std::uint64_t timestamp_us) noexcept override {
        GyroSample sample{};
        sample.timestamp_us = timestamp_us;
        if (!initialized_) {
            sample.status = {.ok = false, .error = Common::DriverError::NotInitialized};
            return sample;
        }

        std::uint8_t reg = config_.data_start_register;
        std::uint8_t bytes[6] = {};
        const auto transfer = transport_.transfer(config_.i2c_address, &reg, 1, bytes, sizeof(bytes));
        if (!transfer.success || transfer.rx_size < sizeof(bytes)) {
            sample.status = {.ok = false, .error = Common::DriverError::TransportError};
            return sample;
        }

        sample.x = static_cast<std::int16_t>((static_cast<std::uint16_t>(bytes[0]) << 8) | bytes[1]);
        sample.y = static_cast<std::int16_t>((static_cast<std::uint16_t>(bytes[2]) << 8) | bytes[3]);
        sample.z = static_cast<std::int16_t>((static_cast<std::uint16_t>(bytes[4]) << 8) | bytes[5]);
        sample.status = {.ok = true, .error = Common::DriverError::None};
        return sample;
    }

    void setSampleCallback(SampleCallback callback) override { callback_ = std::move(callback); }

    Common::DriverStatus pollAndPublish(std::uint64_t timestamp_us) noexcept override {
        const GyroSample sample = readSample(timestamp_us);
        if (callback_) {
            callback_(sample);
        }
        return sample.status;
    }

private:
    Transport& transport_;
    GyroConfig config_{};
    bool initialized_ = false;
    SampleCallback callback_{};
};

} // namespace Drivers::Sensors::Gyroscope
