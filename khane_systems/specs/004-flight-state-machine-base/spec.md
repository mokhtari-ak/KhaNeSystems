# Feature Specification: Basic Flight State Machine (DISARMED, ARMED, FAILSAFE)

**Feature Branch**: `004-flight-state-machine-base`  
**Created**: 2026-05-07  
**Status**: Draft  
**Input**: User description: "Implémenter state machine de base: DISARMED, ARMED, FAILSAFE."

## Drone Scope *(mandatory)*

- **Target Drone Type**: QuadCopter
- **Impacted Modules**: Flight Controller, Propulsion Controller, Telemetry, Logging, RF Communication
- **Boundary Notes**: State machine logic remains application/domain-level and MUST use wrappers/abstractions for hardware/RTOS interactions.
- **PID Notes (if applicable)**: PID outputs are gated by state machine; no propulsion actuation from control outputs when state is `DISARMED` or `FAILSAFE`.
- **EKF/Eigen3 Notes (if applicable)**: EKF validity may be used as a state transition condition or failsafe trigger.
- **Gazebo Sim Notes (if applicable)**: Sim scenarios include arm/disarm transitions and failsafe trigger behavior.
- **Hardware Hook Notes (if applicable)**: Hook-based tests simulate command input, sensor faults, and communication loss to validate transitions.
- **Driver Notes (if applicable)**: Driver fault/status signals must be consumable by state machine guard conditions.

## User Scenarios & Testing *(mandatory)*

### User Story 1 - Safe Arming/Disarming Flow (Priority: P1)

As an operator, I can arm and disarm the drone through explicit commands, with propulsion output allowed only in `ARMED`.

**Why this priority**: Safe actuation gating is the minimum safety boundary before any propulsion-enabled tests.

**Independent Test**: Run command-driven transition tests and verify propulsion output policy per state.

**Acceptance Scenarios**:

1. **Given** state `DISARMED`, **When** valid arm conditions and arm command are received, **Then** state transitions to `ARMED`.
2. **Given** state `ARMED`, **When** disarm command is received, **Then** state transitions to `DISARMED` and propulsion command is forced to safe output.

---

### User Story 2 - Failsafe Entry and Latching (Priority: P2)

As a safety engineer, I need critical faults to force `FAILSAFE` and prevent unsafe actuation.

**Why this priority**: Deterministic failsafe behavior is required to contain sensor/control/communication failures.

**Independent Test**: Inject fault conditions and verify transition to `FAILSAFE`, output policy, and latching behavior.

**Acceptance Scenarios**:

1. **Given** state `ARMED`, **When** a configured critical fault is detected (e.g., control health invalid, communication timeout), **Then** state transitions to `FAILSAFE`.
2. **Given** state `FAILSAFE`, **When** fault clears without explicit reset policy conditions, **Then** state remains `FAILSAFE` (latched behavior).

---

### User Story 3 - Observability and Traceability (Priority: P3)

As a developer, I need clear telemetry/logging of state transitions and reasons.

**Why this priority**: Transition auditability is necessary for debugging and safety validation.

**Independent Test**: Trigger each transition path and verify transition event logging with reason codes and timestamps.

**Acceptance Scenarios**:

1. **Given** any valid transition, **When** state changes, **Then** telemetry/logging records old state, new state, reason code, and timestamp.

---

### Edge Cases

- What happens if arm and failsafe trigger arrive in the same control cycle?
- How does state machine behave with repeated arm commands while already `ARMED`?
- What happens if disarm command arrives during `FAILSAFE`?
- How are transient (single-cycle) fault glitches filtered versus hard faults?

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: System MUST implement a state machine with exactly three base states: `DISARMED`, `ARMED`, `FAILSAFE`.
- **FR-002**: Propulsion command output MUST be gated by state (`ARMED` allowed; `DISARMED`/`FAILSAFE` forced safe output).
- **FR-003**: State transition guards MUST include explicit validity checks and reason codes.
- **FR-004**: Critical fault conditions MUST trigger transition to `FAILSAFE` from any non-failsafe state.
- **FR-005**: `FAILSAFE` behavior MUST be deterministic and follow a documented latching/reset policy.
- **FR-006**: State transitions MUST be logged/telemetered with timestamp, source state, target state, and reason.
- **FR-007**: Integration MUST remain RTOS-agnostic at business logic layer and use abstractions/wrappers only.

### Key Entities *(include if feature involves data)*

- **FlightState**: Enum/type describing `DISARMED`, `ARMED`, `FAILSAFE`.
- **TransitionEvent**: Transition record containing from/to states, reason, and timestamp.
- **StateGuardInput**: Aggregated inputs used for transition decisions (commands, health flags, timeout flags).

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: All valid transition paths execute correctly and all invalid transitions are rejected in automated tests.
- **SC-002**: In `DISARMED` and `FAILSAFE`, propulsion output remains in safe bounded command for 100% of tested cycles.
- **SC-003**: Under injected critical fault scenarios, transition to `FAILSAFE` occurs within configured reaction latency budget.
- **SC-004**: Transition logging coverage is 100% for exercised state changes in test scenarios.

## Assumptions

- Command source (e.g., RF/ground control) provides explicit arm/disarm intents through existing communication layers.
- Safe propulsion output policy is defined by propulsion module constraints.
- Initial implementation targets deterministic baseline behavior; advanced modes (e.g., PREARM/CALIBRATION) are out of scope.
- Formal certification requirements are out of scope for this increment.
