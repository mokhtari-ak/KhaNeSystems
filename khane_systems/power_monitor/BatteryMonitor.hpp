#pragma once

#include <cstdint>
#include "libs/stm32_wrapper/HardwareAccessLayer/IAdcDriver.hpp"

namespace khane::power {

struct BatteryTelemetry {
    float voltage;
    float current;
    bool is_critical;
};

class BatteryMonitor {
public:
    BatteryMonitor(hw::IAdcDriver& adc_v, hw::IAdcDriver& adc_i, float crit_threshold);

    // Read sensors and update telemetry
    void update();

    BatteryTelemetry get_telemetry() const { return telemetry_; }

private:
    hw::IAdcDriver& adc_v_;
    hw::IAdcDriver& adc_i_;
    float crit_threshold_;
    BatteryTelemetry telemetry_;
};

} // namespace khane::power
