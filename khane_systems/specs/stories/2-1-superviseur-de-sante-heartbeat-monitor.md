# Story 2.1: Superviseur de Santé (Heartbeat Monitor)

As a system administrator,
I want a supervisor module to monitor the heartbeat of all critical modules,
So that I can detect and react to module hangs or crashes.

## Description
This story introduces the `Supervisor` module in Epic 2. The module's primary responsibility is to maintain a "heartbeat table" and ensure that all critical system tasks report their activity within a pre-configured time window. If a module stops reporting (timeout), the supervisor must detect this failure and trigger a system-wide alert via the event bus.

## Acceptance Criteria

1. **Heartbeat Registration**:
   - Given a module registers itself at startup,
   - When it registers,
   - Then the `Supervisor` adds it to the internal heartbeat table with its specified expected frequency and timeout window.

2. **Heartbeat Monitoring**:
   - Given a registered critical module,
   - When the module publishes a "Heartbeat" signal,
   - Then the `Supervisor` updates the last-seen timestamp for that module in the heartbeat table.

3. **Timeout Detection**:
   - Given a registered critical module that has failed to publish a heartbeat,
   - When the current time exceeds `last_seen_timestamp` + `configured_timeout_window`,
   - Then the `Supervisor` marks the module as `FAILED` in the heartbeat table.

4. **Alert Triggering**:
   - Given a module is marked as `FAILED`,
   - When the state change occurs,
   - Then the `Supervisor` publishes a `SystemHealthAlert` event (including module ID and error code) to the deterministic event bus.

5. **Resource Efficiency**:
   - Given the system is in steady-state operation,
   - When the `Supervisor` checks the heartbeat table,
   - Then it performs the check without blocking the main event bus or using dynamic memory allocation.

## Technical Notes
- Implement using a static table/array for registered modules to avoid dynamic memory allocation.
- The heartbeat check should be integrated into a high-frequency system task (e.g., the system tick or a dedicated supervisor task).
- Utilize the internal event bus for heartbeat signals and alerts.
