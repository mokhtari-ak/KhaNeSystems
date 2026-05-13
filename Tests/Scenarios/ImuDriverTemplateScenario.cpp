#include <cassert>
#include <cstdint>

#include "Hooks/Common/HookClock.hpp"
#include "Hooks/I2c/I2cHookDriver.hpp"
#include "Libs/Drivers/Sensors/Common/DriverStatus.hpp"
#include "Libs/Drivers/Sensors/Imu/ImuDriver.hpp"

int main() {
    using Drivers::Sensors::Common::DriverError;
    using Drivers::Sensors::Imu::ImuConfig;
    using Drivers::Sensors::Imu::ImuDriver;
    using Tests::Hooks::Common::HookClock;
    using Tests::Hooks::I2c::I2cHookDriver;
    using Tests::Hooks::I2c::I2cHookFault;
    using Tests::Hooks::I2c::I2cTransaction;

    HookClock::reset(0);

    I2cHookDriver i2c{};
    i2c.init(400000);

    // WHO_AM_I
    i2c.scriptTransaction(I2cTransaction{
        .address = 0x68,
        .tx = {0x75},
        .rx = {0x68},
        .success = true,
        .fault = I2cHookFault::None,
    });

    // ACCEL/GYRO raw block
    i2c.scriptTransaction(I2cTransaction{
        .address = 0x68,
        .tx = {0x3B},
        .rx = {
            0x00, 0x10, // ax = 16
            0xFF, 0xF0, // ay = -16
            0x00, 0x20, // az = 32
            0x00, 0x01, // gx = 1
            0x00, 0x02, // gy = 2
            0x00, 0x03  // gz = 3
        },
        .success = true,
        .fault = I2cHookFault::None,
    });

    ImuDriver<I2cHookDriver> imu(i2c, ImuConfig{});
    const auto init_status = imu.init();
    assert(init_status.ok);

    bool callback_called = false;
    imu.setSampleCallback([&callback_called](const auto& cb_sample) {
        callback_called = cb_sample.status.ok;
    });

    HookClock::advanceUs(1000);
    const auto sample = imu.readSample(HookClock::nowUs());
    assert(sample.status.ok);
    assert(sample.timestamp_us == 1000);
    assert(sample.accel_raw.x == 16);
    assert(sample.accel_raw.y == -16);
    assert(sample.accel_raw.z == 32);
    assert(sample.gyro_raw.x == 1);
    assert(sample.gyro_raw.y == 2);
    assert(sample.gyro_raw.z == 3);

    // Push/callback mode
    i2c.scriptTransaction(I2cTransaction{
        .address = 0x68,
        .tx = {0x3B},
        .rx = {
            0x00, 0x01, 0x00, 0x02, 0x00, 0x03,
            0x00, 0x04, 0x00, 0x05, 0x00, 0x06,
        },
        .success = true,
        .fault = I2cHookFault::None,
    });
    HookClock::advanceUs(1000);
    const auto push_status = imu.pollAndPublish(HookClock::nowUs());
    assert(push_status.ok);
    assert(callback_called);

    // Fault path example
    i2c.scriptTransaction(I2cTransaction{
        .address = 0x68,
        .tx = {0x3B},
        .rx = {},
        .success = false,
        .fault = I2cHookFault::Timeout,
    });

    HookClock::advanceUs(1000);
    const auto timeout_sample = imu.readSample(HookClock::nowUs());
    assert(!timeout_sample.status.ok);
    assert(timeout_sample.status.error == DriverError::Timeout);

    return 0;
}
