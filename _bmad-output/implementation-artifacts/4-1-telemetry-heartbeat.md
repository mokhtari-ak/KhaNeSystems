---
story_id: 4.1
story_key: 4-1-telemetry-heartbeat
title: Implémentation du module Telemetry (Heartbeat)
status: ready-for-dev
---

# Story 4.1: Implémentation du module Telemetry (Heartbeat)

## Story Statement
As a flight engineer,
I want the drone to emit a MAVLink heartbeat periodically,
So that the Ground Control Station can detect the drone and monitor its connection status.

## Acceptance Criteria
1. **Given** the system is initialized
   **When** the `Telemetry::update_slow()` is called at 1Hz
   **Then** a MAVLink Heartbeat message is emitted via the configured IBus (UART).
2. **Given** the drone is ARMED
   **When** the heartbeat is sent
   **Then** the status field reflects MAV_STATE_ACTIVE.
3. **Given** the drone is DISARMED
   **When** the heartbeat is sent
   **Then** the status field reflects MAV_STATE_STANDBY.

## Tasks/Subtasks
- [ ] Implement `MavlinkTelemetry::send_heartbeat()` using MAVLink C-library.
- [ ] Integrate into `FlightController::update_slow()`.
- [ ] Add unit tests for heartbeat encoding logic.

## Dev Notes
- **Library**: Use the generated MAVLink C-headers.
- **Bus**: Use `IBus` dependency injection as established in Stage 5.
- **Safety**: Ensure the call is non-blocking and uses the existing static buffer.

## Dev Agent Record
- Implementation Plan: ...
- Debug Log: ...
- Completion Notes: ...

## File List
- `khane_systems/Modules/MavlinkTelemetry.hpp`
- `khane_systems/FlightController/FlightController.hpp`

## Change Log
- (Initial creation)
