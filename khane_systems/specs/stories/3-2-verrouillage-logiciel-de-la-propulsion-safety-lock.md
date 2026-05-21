# Story 3.2: Verrouillage Logiciel de la Propulsion (Safety Lock)

As a system supervisor,
I want a safety lock mechanism for the propulsion subsystem,
So that I can prevent accidental motor activation when the drone is not ARMED.

## Description
This story introduces a software safety lock within the propulsion control layer as part of Epic 3. The mechanism will enforce that motor outputs are strictly set to zero thrust unless the system state is explicitly in the `ARMED` state. Any attempt to send a non-zero thrust command while in any state other than `ARMED` must be blocked, and such attempts must be logged for diagnostics using the Ouroboros logging system.

## Acceptance Criteria

1. **Thrust Command Interception**:
   - Given the `PropulsionController` is active,
   - When a thrust control command is received,
   - Then the controller shall verify the current system state (e.g., via the flight controller state machine).

2. **Safety Enforcement**:
   - Given a thrust control command is received,
   - When the system state is NOT `ARMED`,
   - Then the `PropulsionController` must force the motor output to zero (null thrust), effectively locking the propulsion.

3. **Unauthorized Attempt Logging**:
   - Given the system is not in `ARMED` state,
   - When a non-zero thrust command is issued,
   - Then the `PropulsionController` shall generate a diagnostic log entry via the Ouroboros logging system, detailing the unauthorized attempt and the current state.

4. **Resource Efficiency**:
   - Given the system is in steady-state operation,
   - When the safety check is performed,
   - Then it shall operate without blocking the main control loop and without any dynamic memory allocation.

## Technical Notes
- The `ARMED` state should be queried from the `FlightState` module or equivalent.
- The `PropulsionController` is responsible for applying the safety logic before writing to the PWM (Pulse Width Modulation) outputs.
- Logging should follow the project convention for non-blocking telemetry and error reporting.
- Ensure the safety lock takes precedence over any PID control input.
