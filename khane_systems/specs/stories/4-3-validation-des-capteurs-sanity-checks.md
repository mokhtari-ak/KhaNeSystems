# Story 4.3: Validation des capteurs (Sanity Checks)

As an Autopilot Developer,
I want a validation layer to perform sanity checks on incoming sensor data (GNSS, IMU),
So that invalid, inconsistent, or extreme data is flagged and ignored by the SensorFusion module before it can corrupt the filter state.

## Description
This story introduces a sanity-check validation layer as a prerequisite for `SensorFusion`. The module will act as a gatekeeper, processing raw sensor frames from the internal bus. It will apply domain-specific heuristics and threshold-based validation to verify data physical consistency and signal quality. Invalid frames will be tagged or discarded, preventing the EKF from diverging due to sensor noise or hardware failure.

## Acceptance Criteria

1. **Validation Pipeline Integration**:
   - Given a stream of raw sensor data (IMU, GNSS),
   - When data is received by the validation layer,
   - Then each frame is checked against defined safety heuristics before being passed to `SensorFusion`.

2. **GNSS Consistency Heuristics**:
   - Given incoming GNSS data,
   - When the HDOP (Horizontal Dilution of Precision) exceeds a defined threshold or values are physically implausible (e.g., negative altitude without descent),
   - Then the data is flagged as "Invalid" and filtered out.

3. **IMU Physical Limit Checks**:
   - Given incoming IMU acceleration and gyro data,
   - When the values exceed the physical capabilities of the quadcopter (e.g., impossible angular velocity or linear acceleration spikes),
   - Then the data is flagged as "Invalid" and discarded.

4. **Invalid Data Signaling**:
   - Given an invalid sensor frame,
   - When it is detected,
   - Then the validation layer signals the error to the `SensorFusion` module or updates a shared status topic, allowing the system to react (e.g., switch to IMU-only mode).

5. **Resource Determinism**:
   - Given the validation layer is in the high-frequency control loop,
   - When processing incoming frames,
   - Then the sanity checks execute in O(1) time complexity without dynamic memory allocation, respecting real-time constraints.

## Technical Notes
- The validation layer should be implemented as a stateless or low-state filter/checker.
- Thresholds for consistency (HDOP, max angular rate, etc.) should be configurable through a centralized parameter store or configuration file.
- Sensor status flags must be communicated via the internal event bus.
- Prioritize clear, maintainable code for the heuristic logic to facilitate future addition of new checks.
