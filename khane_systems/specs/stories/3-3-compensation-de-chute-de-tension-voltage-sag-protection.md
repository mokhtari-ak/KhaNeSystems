# Story 3.3: Compensation de Chute de Tension (Voltage Sag Protection)

As a drone flight controller,
I want to automatically limit the thrust slew rate when battery voltage approaches a critical threshold,
So that I can prevent a brownout of the electronics and ensure continued system stability under high motor load.

## Description
This story implements the `VoltageSagProtection` module in Epic 3. The module's primary responsibility is to subscribe to `BatteryTelemetry` events from the `BatteryMonitor` and, if the battery voltage approaches the critical threshold, dynamically modulate the motor control command stream to enforce a maximum allowable thrust slew rate (limit the rate of change of thrust). This prevents sudden current spikes that cause sharp voltage drops (sags) and potentially reset the flight controller.

## Acceptance Criteria

1. **Telemetry Subscription**:
   - Given the `VoltageSagProtection` module is active,
   - When it receives a `BatteryTelemetry` event,
   - Then it shall update its internal model of the battery state, specifically monitoring for voltage sags.

2. **Thrust Slew Rate Limiting**:
   - Given the system is operating near the `VOLTAGE_WARNING_THRESHOLD` (set above the critical threshold),
   - When the battery voltage trends downwards towards the critical threshold,
   - Then the `VoltageSagProtection` module shall dynamically adjust the maximum allowable thrust slew rate to a value inversely proportional to the severity of the sag.

3. **Electronics Stability Prioritization**:
   - Given a high-performance maneuver is requested,
   - When the battery voltage is below the `VOLTAGE_WARNING_THRESHOLD`,
   - Then the protection module shall override the requested motor outputs, capping the change in thrust to prevent the voltage from dropping below the `VOLTAGE_BROWNOUT_THRESHOLD`.

4. **Alert & Reporting**:
   - Given a thrust slew rate limitation is active,
   - When the system reaches the sag-limited state,
   - Then the module shall publish a `VoltageSagProtectionActive` alert to the event bus, informing the flight supervisor that maximum performance is currently throttled.

5. **Resource Efficiency**:
   - Given the high frequency of motor control updates,
   - When the `VoltageSagProtection` processes telemetry and applies limits,
   - Then it shall perform these operations with deterministic execution time, avoiding any blocking or memory allocation, ensuring real-time performance.

## Technical Notes
- The warning and critical thresholds must be configurable parameters.
- The slew rate limiting algorithm must be simple, efficient, and deterministic (e.g., linear ramp rate reduction based on voltage deviation).
- The implementation must seamlessly integrate with the motor command path, likely by intercepting or wrapping the output control signals before they reach the PWM/ESC drivers.
- Ensure the mechanism can be disabled or bypassed if necessary for emergency flight maneuvers (configurable).
