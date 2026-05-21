---
epic: Epic 2
story: 2-2
title: Gestionnaire de Failsafe multiniveaux
assignedTo: Amélia (Agent-Dev)
status: To Do
---

# Story 2-2: Gestionnaire de Failsafe multiniveaux

## Description
As a safety officer,
I want a multi-level failsafe manager to execute graduated safety actions,
So that the drone reacts appropriately to different types of failures received from the Supervisor.

## Acceptance Criteria
1. **Given** the Supervisor has detected a failure (e.g., RC loss, battery low, EKF divergence) and published a `FailsafeRequest` event on the event bus
   **When** the `FailsafeManager` receives this request
   **Then** it evaluates the severity level (INFO, WARNING, CRITICAL, FATAL) associated with the alert.

2. **Given** a `CRITICAL` severity failure (e.g., major sensor timeout)
   **When** the `FailsafeManager` processes the request
   **Then** it triggers an immediate transition of the main state machine to `FAILSAFE`
   **And** executes the pre-configured "Controlled Landing" sequence (descent with yaw stabilization).

3. **Given** a `FATAL` severity failure (e.g., EKF divergence or propulsion loss)
   **When** the `FailsafeManager` processes the request
   **Then** it triggers an immediate `DISARM` of the propulsion system
   **And** logs the specific failure origin and timestamp for post-flight analysis.

4. **Given** a `WARNING` severity failure (e.g., link degradation)
   **When** the `FailsafeManager` processes the request
   **Then** it triggers a "Return to Launch" (RTL) behavior without disarming the propulsion.

5. **Given** multiple simultaneous failure alerts
   **When** the `FailsafeManager` processes them
   **Then** it prioritizes the action corresponding to the highest severity failure among the active alerts.

## Technical Notes
- Implement as a subscriber to the `FailsafeRequest` topic on the event bus.
- State transitions must be coordinated with the main Flight State Machine.
- Must be non-blocking and deterministic (no dynamic memory allocation).
- All actions taken must be logged via the Ouroboros logging system (Story 1.2).
