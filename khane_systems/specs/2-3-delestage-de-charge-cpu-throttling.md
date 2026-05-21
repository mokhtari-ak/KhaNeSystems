---
epic: Epic 2
story: 2.3
title: Délestage de Charge CPU (Throttling)
priority: High
status: Defined
---

# Story 2.3: Délestage de Charge CPU (Throttling)

## Description
As a flight controller,
I want the supervisor to throttle non-critical tasks during CPU spikes,
So that my high-priority control loops remain deterministic.

## Rationale
To ensure the stability and safety of the drone, high-priority tasks (Attitude control, EKF) must meet their real-time deadlines. When system load (CPU usage) exceeds a safe threshold (e.g., 85%), background or non-critical processes must reduce their resource consumption (frequency) to preserve the real-time determinism required for flight safety.

## Acceptance Criteria

1. **Given** a measured CPU load exceeding the predefined threshold (e.g., 85%),
   **When** the Supervisor activates the load-shedding policy,
   **Then** non-critical tasks (Telemetry, Logging) are throttled, reducing their execution frequency to a predefined "reduced" state.

2. **Given** the CPU load returns to a healthy level (below the threshold + hysteresis),
   **When** the Supervisor deactivates the load-shedding policy,
   **Then** non-critical tasks must automatically resume their nominal, high-frequency execution rate.

3. **Given** any CPU load state,
   **When** the system is running,
   **Then** the Flight Control task (Attitude/Propulsion control) MUST maintain its nominal frequency and priority, completely unaffected by the throttling of non-critical modules.

4. **Given** a throttling event,
   **When** the load-shedding policy is activated or deactivated,
   **Then** a diagnostic event (LoadSheddingEvent) must be published to the Internal Event Bus to ensure transparency and auditability of system performance.

## Implementation Notes
- Use a predefined enum for Task Priority levels.
- The Supervisor must monitor the load via periodic sampling of the RTOS runtime statistics or a cycle counter.
- The mechanism for frequency adjustment should be implemented within the task schedulers or by modifying the task period via the RTOS abstraction layer.
- Throttling configuration (thresholds, frequency levels) must be managed by the Parameter Server (Story 5.1).
