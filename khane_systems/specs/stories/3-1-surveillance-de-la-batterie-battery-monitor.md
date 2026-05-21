# Story 3.1: Surveillance de la Batterie (Battery Monitor)

As a drone pilot,
I want a battery monitor module to track voltage and current,
So that I can be alerted when the power supply is critical.

## Description
This story introduces the `BatteryMonitor` module in Epic 3. The module's primary responsibility is to interface with the ADC (Analog-to-Digital Converter) to measure battery voltage and current. It will process these raw values, publish periodic telemetry updates to the internal deterministic event bus, and detect voltage sags below a pre-configured safety threshold to trigger a 'Battery Critical' alert.

## Acceptance Criteria

1. **ADC Integration & Sampling**:
   - Given the battery monitor is initialized,
   - When it performs a sampling cycle,
   - Then it shall read raw data from the configured ADC channels for voltage and current and convert them to engineering units.

2. **Telemetry Publication**:
   - Given valid sampled data,
   - When the sampling interval is reached,
   - Then the `BatteryMonitor` publishes a `BatteryTelemetry` event (containing voltage and current) to the deterministic event bus.

3. **Critical Threshold Monitoring**:
   - Given the voltage measurement,
   - When the voltage drops below the pre-configured `VOLTAGE_CRITICAL_THRESHOLD`,
   - Then the `BatteryMonitor` publishes a `BatteryCriticalAlert` event (containing the current voltage) to the event bus.

4. **Resource Efficiency**:
   - Given the system is in steady-state operation,
   - When the `BatteryMonitor` samples data,
   - Then it shall operate using a non-blocking sampling loop, avoiding busy-waiting or dynamic memory allocation.

## Technical Notes
- Use the HAL or low-level register access as defined by the project's abstraction layer to interface with the ADC.
- Ensure the sampling rate is high enough for reliable reading but low enough not to impact system CPU budget (configurable).
- The alert threshold must be configurable via a header file or a configuration structure.
- Utilize the internal event bus for all communication.
