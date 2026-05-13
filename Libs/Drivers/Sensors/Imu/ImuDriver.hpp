#pragma once

#include <cstddef>
#include <cstdint>
#include <utility>

#include "IImuDriver.hpp"

namespace Drivers::Sensors::Imu {

template <typename Transport>
class ImuDriver final : public IImuDriver {
public:
    ImuDriver(Transport& transport, ImuConfig config) noexcept
        : transport_(transport), config_(config) {}

    Common::DriverStatus init() noexcept override {
        std::uint8_t reg = config_.who_am_i_register;
        std::uint8_t value = 0;
        const auto transfer = transport_.transfer(config_.i2c_address, &reg, 1, &value, 1);
        if (!transfer.success) {
            return {.ok = false, .error = mapTransportError(transfer.fault)};
        }
        if (value != config_.expected_who_am_i) {
            return {.ok = false, .error = Common::DriverError::DeviceNotFound};
        }
        initialized_ = true;
        return {.ok = true, .error = Common::DriverError::None};
    }

    ImuSample readSample(std::uint64_t timestamp_us) noexcept override {
        ImuSample sample{};
        sample.timestamp_us = timestamp_us;

        if (!initialized_) {
            sample.status = {.ok = false, .error = Common::DriverError::NotInitialized};
            return sample;
        }

        std::uint8_t reg = config_.data_start_register;
        std::uint8_t bytes[12] = {};
        const auto transfer = transport_.transfer(config_.i2c_address, &reg, 1, bytes, sizeof(bytes));
        if (!transfer.success || transfer.rx_size < sizeof(bytes)) {
            sample.status = {.ok = false, .error = mapTransportError(transfer.fault)};
            return sample;
        }

        sample.accel_raw = decodeVec3(bytes + 0);
        sample.gyro_raw = decodeVec3(bytes + 6);
        sample.status = {.ok = true, .error = Common::DriverError::None};
        return sample;
    }

    void setSampleCallback(SampleCallback callback) override { callback_ = std::move(callback); }

    Common::DriverStatus pollAndPublish(std::uint64_t timestamp_us) noexcept override {
        const ImuSample sample = readSample(timestamp_us);
        if (callback_) {
            callback_(sample);
        }
        return sample.status;
    }

private:
    static Vec3i16 decodeVec3(const std::uint8_t* p) noexcept {
        return {
            .x = static_cast<std::int16_t>((static_cast<std::uint16_t>(p[0]) << 8) | p[1]),
            .y = static_cast<std::int16_t>((static_cast<std::uint16_t>(p[2]) << 8) | p[3]),
            .z = static_cast<std::int16_t>((static_cast<std::uint16_t>(p[4]) << 8) | p[5]),
        };
    }

    static Common::DriverError mapTransportError(typename Transport::FaultType fault) noexcept {
        if (fault == Transport::FaultType::Timeout) {
            return Common::DriverError::Timeout;
        }
        return Common::DriverError::TransportError;
    }

    Transport& transport_;
    ImuConfig config_{};
    bool initialized_ = false;
    SampleCallback callback_{};
};

} // namespace Drivers::Sensors::Imu
