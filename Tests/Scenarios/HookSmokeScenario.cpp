#include <cassert>
#include <cstdint>

#include "Hooks/Adc/AdcHookDriver.hpp"
#include "Hooks/Common/HookClock.hpp"
#include "Hooks/I2c/I2cHookDriver.hpp"

int main() {
    using Tests::Hooks::Adc::AdcHookDriver;
    using Tests::Hooks::Adc::AdcHookFault;
    using Tests::Hooks::Common::HookClock;
    using Tests::Hooks::I2c::I2cHookDriver;
    using Tests::Hooks::I2c::I2cHookFault;
    using Tests::Hooks::I2c::I2cTransaction;

    HookClock::reset(1000);
    assert(HookClock::nowUs() == 1000);
    HookClock::advanceUs(500);
    assert(HookClock::nowUs() == 1500);

    I2cHookDriver i2c{};
    i2c.init(400000);
    i2c.scriptTransaction(I2cTransaction{
        .address = 0x68,
        .tx = {0x75},
        .rx = {0x42},
        .success = true,
        .fault = I2cHookFault::None,
    });

    std::uint8_t tx[1] = {0x75};
    std::uint8_t rx[1] = {0};
    const auto i2c_result = i2c.transfer(0x68, tx, 1, rx, 1);
    assert(i2c_result.success);
    assert(i2c_result.rx_size == 1);
    assert(rx[0] == 0x42);

    AdcHookDriver adc{};
    adc.init();
    adc.pushSample(0, 1234);

    const auto adc_ok = adc.read(0);
    assert(adc_ok.success);
    assert(adc_ok.sample == 1234);

    adc.setFault(0, AdcHookFault::Timeout);
    const auto adc_timeout = adc.read(0);
    assert(!adc_timeout.success);
    assert(adc_timeout.fault == AdcHookFault::Timeout);

    return 0;
}
