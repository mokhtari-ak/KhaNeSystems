#include "BatteryMonitor.hpp"

namespace khane::power {

BatteryMonitor::BatteryMonitor(hw::IAdcDriver& adc_v, hw::IAdcDriver& adc_i, float crit_threshold)
    : adc_v_(adc_v), adc_i_(adc_i), crit_threshold_(crit_threshold) {}

void BatteryMonitor::update() {
    telemetry_.voltage = adc_v_.read_value() * 3.3f / 4095.0f * 10.0f; // Example scaling
    telemetry_.current = adc_i_.read_value() * 3.3f / 4095.0f * 5.0f;  // Example scaling
    telemetry_.is_critical = (telemetry_.voltage < crit_threshold_);
}

} // namespace khane::power
