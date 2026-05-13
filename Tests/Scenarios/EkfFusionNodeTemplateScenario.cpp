#include <cassert>
#include <cstdint>

#include "Hooks/Common/HookClock.hpp"
#include "Hooks/I2c/I2cHookDriver.hpp"
#include "Libs/Drivers/SensorFusion/EkfFusionNode.hpp"
#include "Libs/Drivers/SensorFusion/ImuDriverInputAdapter.hpp"
#include "Libs/Drivers/SensorFusion/EkfTuning.hpp"
#include "Libs/Drivers/Sensors/Imu/ImuDriver.hpp"

int main() {
    using Drivers::SensorFusion::EkfFusionConfig;
    using Drivers::SensorFusion::EkfFusionNode;
    using Drivers::SensorFusion::EkfProfile;
    using Drivers::SensorFusion::EkfTuning;
    using Drivers::SensorFusion::ImuDriverInputAdapter;
    using Drivers::Sensors::Imu::ImuConfig;
    using Drivers::Sensors::Imu::ImuDriver;
    using Tests::Hooks::Common::HookClock;
    using Tests::Hooks::I2c::I2cHookDriver;
    using Tests::Hooks::I2c::I2cHookFault;
    using Tests::Hooks::I2c::I2cTransaction;

    HookClock::reset(0);

    I2cHookDriver i2c{};
    i2c.init(400000);
    i2c.scriptTransaction(I2cTransaction{
        .address = 0x68,
        .tx = {0x75},
        .rx = {0x68},
        .success = true,
        .fault = I2cHookFault::None,
    });

    i2c.scriptTransaction(I2cTransaction{
        .address = 0x68,
        .tx = {0x3B},
        .rx = {
            0x00, 0x00, 0x00, 0x00, 0x40, 0x00,
            0x00, 0x0A, 0x00, 0x14, 0x00, 0x1E,
        },
        .success = true,
        .fault = I2cHookFault::None,
    });

    ImuDriver<I2cHookDriver> imu(i2c, ImuConfig{});
    const auto imu_init = imu.init();
    assert(imu_init.ok);

    ImuDriverInputAdapter adapter(imu);
    EkfFusionConfig cfg = EkfTuning::forProfile(EkfProfile::Sim);
    cfg.gyro_scale = 0.1f;
    EkfFusionNode node(adapter, cfg);
    node.init();

    HookClock::advanceUs(1000);
    const auto& state = node.step(HookClock::nowUs());
    assert(state.valid);
    assert(state.timestamp_us == 1000);
    assert(state.roll > 0.0f);
    assert(state.pitch > 0.0f);
    assert(state.yaw > 0.0f);
    assert(state.roll < 1.0f);
    assert(state.pitch < 2.0f);
    assert(state.yaw < 3.0f);

    return 0;
}
