# Feature Specification: Closed-Loop Integration IMU + EKF + PID + Propulsion

**Feature Branch**: `003-closed-loop-imu-ekf-pid-propulsion`  
**Created**: 2026-05-07  
**Status**: Draft  
**Input**: User description: "Intégre ImuDriver + EkfFusionNode + PID + Propulsion en chaîne fermée."

## Drone Scope *(mandatory)*

- **Target Drone Type**: QuadCopter
- **Impacted Modules**: Flight Controller, PID Controller, Propulsion Controller, Sensor Fusion, Telemetry, Logging
- **Boundary Notes**: Hardware access remains wrapper-only (I2C/SPI/UART/GPIO/PWM wrappers). No direct HAL/RTOS calls in application logic.
- **PID Notes (if applicable)**: Loop rates, output saturation, anti-windup behavior, and mode transition handling are explicitly documented.
- **EKF/Eigen3 Notes (if applicable)**: EKF state/model/covariance assumptions are documented; tuning profile and fallback behavior are defined.
- **Gazebo Sim Notes (if applicable)**: Closed-loop scenario includes nominal hover, disturbance rejection, and measurable pass/fail thresholds.
- **Hardware Hook Notes (if applicable)**: Hook-driven hardware simulation maps IMU and propulsion paths to production wrappers.
- **Driver Notes (if applicable)**: IMU driver timing, data validity, and fault propagation are documented with explicit status model.

## User Scenarios & Testing *(mandatory)*

### User Story 1 - End-to-End Closed Loop (Priority: P1)

As a flight-control engineer, I run a closed-loop chain where IMU samples feed EKF, EKF state feeds PID, and PID outputs feed propulsion commands.

**Why this priority**: This is the minimum viable integration proving that core flight-control modules work together as one deterministic loop.

**Independent Test**: Execute a fixed-rate loop test (Hook or Gazebo) and verify valid IMU->EKF->PID->Propulsion data flow with bounded timing and non-zero actuator outputs.

**Acceptance Scenarios**:

1. **Given** valid IMU data stream, **When** the control loop runs at configured frequency, **Then** EKF state updates and PID outputs are produced each cycle without contract violations.
2. **Given** stable target attitude, **When** loop runs for configured duration, **Then** propulsion commands remain bounded and consistent with PID correction direction.

---

### User Story 2 - Disturbance Handling in Closed Loop (Priority: P2)

As a control developer, I inject attitude disturbances and verify that the closed-loop chain corrects state while respecting saturation and anti-windup constraints.

**Why this priority**: Disturbance response validates practical control robustness and safe actuator behavior under transient errors.

**Independent Test**: Apply controlled disturbance in simulation and verify convergence/recovery metrics plus bounded propulsion outputs.

**Acceptance Scenarios**:

1. **Given** hover-equivalent stabilized state, **When** a disturbance is injected, **Then** EKF tracks the deviation and PID/propulsion drive recovery within configured limits and timeout.

---

### User Story 3 - Sensor Fault Propagation and Safe Behavior (Priority: P3)

As a safety reviewer, I verify that IMU faults propagate through EKF/PID chain and trigger safe output behavior.

**Why this priority**: Fault transparency and controlled degradation are required before any hardware flight validation.

**Independent Test**: Inject IMU timeout/invalid data via hooks and verify health/status propagation, degraded control behavior, and logging.

**Acceptance Scenarios**:

1. **Given** IMU transport timeout or invalid sample, **When** the loop executes, **Then** fault status propagates to fusion/control layers and propulsion output follows defined safe behavior.

---

### Edge Cases

- What happens when IMU timestamp jitter exceeds nominal loop period?
- How does EKF behave when accelerometer norm is invalid or near zero?
- What happens when PID output is saturated for extended windows?
- How is propulsion command handling defined when EKF state is invalid?

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: System MUST integrate `ImuDriver`, `EkfFusionNode`, PID controller, and propulsion command path in a deterministic closed-loop pipeline.
- **FR-002**: System MUST enforce wrapper-only hardware access for IMU and propulsion-related I/O.
- **FR-003**: Loop execution MUST use explicit timestamps and configured update rates for IMU read, EKF step, PID compute, and propulsion output update.
- **FR-004**: System MUST propagate status/health from IMU driver to EKF and control layers without silent fault masking.
- **FR-005**: PID output MUST be bounded by configured propulsion limits and anti-windup strategy.
- **FR-006**: System MUST log/telemeter per-cycle or sampled diagnostic fields (timing, EKF validity, PID output, propulsion command).
- **FR-007**: EKF tuning profile selection (`SIM`/`BENCH`/`FLIGHT`) MUST be configurable and traceable.
- **FR-008**: Closed-loop integration MUST support both pull-mode and callback/push-mode IMU acquisition paths.

### Key Entities *(include if feature involves data)*

- **ClosedLoopFrame**: Single-cycle integration frame containing timestamp, IMU sample, EKF state, PID outputs, and propulsion command.
- **ControlHealthStatus**: Aggregated validity/error status from sensor, fusion, and control stages for decision and failsafe use.
- **PropulsionCommand**: Structured bounded output command for actuators/motors with source metadata and cycle timestamp.

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: Closed-loop pipeline executes at target frequency with no missed cycle budget beyond defined tolerance during nominal run window.
- **SC-002**: In nominal scenario, EKF validity remains true for at least 99% of executed cycles.
- **SC-003**: Under disturbance scenarios, attitude error converges back within configured envelope before recovery timeout.
- **SC-004**: Under injected IMU fault scenarios, fault propagation and safe output behavior are observed and logged in 100% of test runs.

## Assumptions

- Existing wrappers and driver templates are available and can be composed without breaking current architecture boundaries.
- Initial PID and EKF profile values are sufficient for first integration pass and will be tuned iteratively.
- Propulsion interface supports bounded command output suitable for closed-loop integration testing.
- Certification-grade evidence is out of scope for this feature; engineering validation discipline is in scope.
