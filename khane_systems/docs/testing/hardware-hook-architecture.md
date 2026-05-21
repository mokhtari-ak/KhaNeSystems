# Hardware Hook Test Architecture (STM32 Drone Project)

## Objective

Provide a deterministic test architecture that simulates hardware through Hook classes while keeping production code on wrapper interfaces.

## Integration Principles

- Application and control modules depend on wrapper contracts only.
- Production path uses `Libs/STM32Wrapper` implementations.
- Test/simulation path uses Hook implementations with the same contracts.
- No direct HAL dependency in test logic.

## Proposed Directory Layout

```text
Libs/
  Wrappers/
    ... existing static wrappers and configs ...

Tests/
  Hooks/
    Common/
      HookClock.hpp
      HookEventBus.hpp
      HookFaultModel.hpp
    Gpio/
      GpioHookDriver.hpp
    I2c/
      I2cHookDriver.hpp
    Spi/
      SpiHookDriver.hpp
    Uart/
      UartHookDriver.hpp
    Can/
      CanHookDriver.hpp
    Adc/
      AdcHookDriver.hpp
    Pwm/
      PwmHookDriver.hpp
  Fixtures/
    FlightControlFixture.hpp
    SensorFusionFixture.hpp
  Scenarios/
    HoverNominalScenario.cpp
    DisturbanceRecoveryScenario.cpp
    SensorFaultScenario.cpp
```

## Hook Class Model

Each Hook driver mirrors the production wrapper-facing API and adds test controls.

- Required surface:
  - `init(...)`
  - runtime I/O methods (`read`, `write`, `transfer`, etc.)
  - optional IRQ callback registration
- Test controls:
  - set scripted inputs
  - inject failures (`timeout`, `crc`, `bus_error`, `stuck_value`)
  - collect call trace and timestamps

Example behavior for `I2cHookDriver`:
- queue expected transactions
- return deterministic responses per step
- fail on configured call index
- expose transaction log for assertions

## Time and Scheduling

Use a virtual clock for deterministic progression:

- `HookClock::nowUs()`
- `HookClock::advanceUs(delta)`
- `HookClock::sleepUntil(t)`

Control loops and EKF update functions consume this clock in tests so replay is stable.

## Fault Injection

Central fault model:

- `HookFaultModel::enable(name)`
- `HookFaultModel::disable(name)`
- `HookFaultModel::setRate(name, p)`

Common faults:
- Sensor dropout
- Latency spikes
- Stale samples
- Bus timeout
- Saturated actuator command acknowledgment

## Wrapper/Hook Mapping

- `GpioStatic` -> `GpioHookDriver`
- `I2cStatic` -> `I2cHookDriver`
- `SpiStatic` -> `SpiHookDriver`
- `UartStatic` -> `UartHookDriver`
- `CanStatic` -> `CanHookDriver`
- `AdcStatic` -> `AdcHookDriver`
- `PwmStatic` -> `PwmHookDriver`

Rule: behaviorally equivalent contract, test-only observability extras allowed.

## Injection Strategy

Use compile-time aliasing for minimal runtime cost:

- Production build:
  - `using I2cDriver = Stm32I2cDriver;`
- Test build:
  - `using I2cDriver = I2cHookDriver;`

For modules already template-based, inject driver/config type directly in test fixtures.

## CMake Targets (Suggested)

- `khane_wrappers_prod` (existing production path)
- `khane_hooks` (new static lib for Hook drivers)
- `khane_test_fixtures` (fixtures + scenario helpers)
- `khane_sim_tests` (unit/integration executable)

Suggested definitions:
- `KHANE_TEST_HOOKS=1`
- `KHANE_SIMULATION=1`

## Validation Levels

1. Contract tests (wrapper vs hook parity)
2. Module tests (PID, Sensor Fusion, Telemetry with hooks)
3. Scenario tests (hover, disturbance, sensor fault)
4. Gazebo correlation tests (compare key metrics against Hook-based runs)

## Minimal First Iteration

1. Implement `HookClock`, `I2cHookDriver`, `AdcHookDriver`, `PwmHookDriver`.
2. Cover `Sensor Fusion` + `PID Controller` core loops.
3. Add three scenarios:
   - nominal hover
   - disturbance recovery
   - IMU dropout fallback
4. Emit report artifacts: metrics + pass/fail + fault timeline.

## Acceptance Criteria for This Architecture

- A control loop can run end-to-end without physical hardware.
- Same module code compiles with prod wrappers and hook wrappers.
- Fault injection is reproducible by seed/config.
- Test outputs are machine-checkable (threshold-based verdicts).
