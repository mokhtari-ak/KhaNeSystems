# Story 4.4: EKF Adaptatif selon la charge CPU

As an Autopilot Developer,
I want the EKF to adapt its computational model based on the system load (provided by `CpuThrottler`),
So that the navigation system remains deterministic and maintains real-time control constraints even under high CPU load.

## Description
This story implements an adaptive execution strategy for the Sensor Fusion module (EKF). The EKF will monitor the system's CPU load via the `CpuThrottler`. When the CPU load exceeds a defined critical threshold, the EKF will switch from a full navigation state estimation (including GPS/Position integration) to a reduced attitude-only state estimation. This ensures that the essential orientation control loop execution time remains predictable and within real-time deadlines during heavy processing, preventing system-wide instability.

## Acceptance Criteria

1. **CPU Load Integration**:
   - Given the `SensorFusion` module is active,
   - When it polls the `CpuThrottler` state,
   - Then it identifies whether the system is in a "normal" or "high-load" operating state.

2. **Dynamic Model Switching**:
   - Given a transition from a "normal" to "high-load" state,
   - When the next EKF update cycle begins,
   - Then the `SensorFusion` module switches to the reduced attitude-only EKF model within a single iteration, ensuring no control loop missed deadlines.

3. **Determinism Maintenance**:
   - Given the system is under heavy load,
   - When the attitude-only model is active,
   - Then the EKF execution time remains within the strictly defined real-time limit, and covariance estimation is limited only to attitude-relevant states.

4. **Seamless Transition**:
   - Given a transition back to a "normal" load state,
   - When the system detects sufficient headroom,
   - Then the `SensorFusion` module re-initializes and seamlessly reintegrates the full navigation (position/velocity) estimation states without causing discontinuous spikes in output data.

5. **Interface Stability**:
   - Given the dynamic switching mechanism,
   - When consumers request navigation state data,
   - Then they receive the best available state information (with updated covariance reflecting the model being used) through the standard `SensorFusion` API.

## Technical Notes
- The logic must be triggered within the existing real-time control task.
- The state transition logic (e.g., re-initialization of matrices when switching) must be carefully implemented to avoid numerical instability.
- Ensure the `CpuThrottler` interaction follows the non-blocking policy established for internal communication.
- The `SensorFusion` module must maintain consistent interface output despite internal model changes.
