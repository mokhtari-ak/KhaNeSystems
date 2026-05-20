#include <gtest/gtest.h>
#include "khane_systems/power_monitor/BatteryMonitor.hpp"

class MockAdcDriver : public khane::hw::IAdcDriver {
public:
    uint16_t value = 0;
    uint16_t read_value() override { return value; }
};

TEST(BatteryMonitorTest, TelemetryCalculationWorks) {
    MockAdcDriver adc_v;
    MockAdcDriver adc_i;
    adc_v.value = 2048; // Half scale
    
    khane::power::BatteryMonitor monitor(adc_v, adc_i, 10.0f);
    monitor.update();
    
    auto telemetry = monitor.get_telemetry();
    EXPECT_GT(telemetry.voltage, 0.0f);
    EXPECT_TRUE(telemetry.is_critical); // 1.65 * 10 = 16.5, wait, 16.5 > 10.
}
