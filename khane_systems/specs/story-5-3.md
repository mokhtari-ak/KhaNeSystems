# Story 5.3: Exécuteur de Mission par Waypoints

Status: ready-for-dev

## Story

As a mission operator,
I want the drone to autonomously navigate through GPS waypoints,
so that complex flight paths can be executed without manual stick input.

## Acceptance Criteria

1. The Navigation module shall maintain an internal buffer of waypoint sequences (Lat/Lon/Alt).
2. Given a list of valid GPS waypoints and an ARMED state, the module shall continuously calculate and update the heading and altitude setpoints to reach the target waypoint.
3. The module shall implement safety checks to evaluate the reachability of a waypoint based on current telemetry (e.g., distance, required energy).
4. If a waypoint is determined to be unreachable (e.g., energy consumption exceeds threshold, waypoint is too far), the system shall trigger an abort and transition to FAILSAFE mode.
5. Navigation logic shall be implemented to account for the platform-specific flight dynamics (Quadcopter vs Fixed-wing as per Epic 5.2).

## Tasks / Subtasks

- [ ] Task 1: Navigation Module Scaffolding (AC: #1)
  - [ ] Implement waypoint list data structure.
  - [ ] Integrate with the event bus for waypoint management commands.
- [ ] Task 2: Trajectory Setpoint Calculation (AC: #2, #5)
  - [ ] Implement waypoint-to-setpoint algorithm (heading/altitude).
  - [ ] Ensure adaptation based on vehicle type (static polymorphism).
- [ ] Task 3: Safety/Abort Logic Implementation (AC: #3, #4)
  - [ ] Implement energy/reachability estimator.
  - [ ] Integrate abort trigger with Supervisor/Failsafe state machine.

## Dev Notes

- Relevant architecture patterns: Static polymorphism for flight dynamics (Epic 5.2), event-bus based command interface.
- Source tree: `khane_systems/navigation/`, `khane_systems/FlightController/`.
- Testing: Unit testing for waypoint calculation (scenarios/navigation).

### Project Structure Notes

- Alignment with Epic 5.

### References

- [Source: _bmad-output/planning-artifacts/epics.md#Epic-5]

## Dev Agent Record

### Agent Model Used

Gemini 1.5 Pro

### Completion Notes List

### File List
