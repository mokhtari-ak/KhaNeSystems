# Story 4.1: Interface Sensor Fusion Encapsulation (EKF)

As an Autopilot Developer,
I want to encapsulate the `KalmanFilterLibrary` within a standardized Sensor Fusion module,
So that the rest of the system can access orientation data (quaternions/Euler angles) through a consistent interface without being coupled to the underlying filtering implementation.

## Description
This story introduces the `SensorFusion` module in Epic 4. The primary goal is to hide the complexity of the `KalmanFilterLibrary` behind an abstraction layer. This module will ingest raw IMU, magnetometer, and GPS data (from the internal communication bus) and expose high-level orientation state and covariance data to the navigation and flight control loops.

## Acceptance Criteria

1. **Abstraction Layer Interface**:
   - Given a request for orientation state,
   - When the `SensorFusion` module receives it,
   - Then it provides current attitude in both quaternion and Euler angle formats via a stable, consistent API, hiding the EKF implementation.

2. **Input Normalization**:
   - Given raw sensor data from the internal bus,
   - When new data is received,
   - Then the `SensorFusion` module normalizes the inputs and feeds them into the `KalmanFilterLibrary` state update function.

3. **Output Standardization**:
   - Given an update from the EKF,
   - When the state is queried,
   - Then the provided state includes associated covariance data to indicate sensor fusion confidence.

4. **Implementation Hiding**:
   - Given the system depends on the `SensorFusion` module,
   - When the underlying `KalmanFilterLibrary` or filter algorithm is upgraded or changed,
   - Then the public API of the `SensorFusion` module remains unchanged, ensuring no downstream dependencies require modifications.

5. **Resource Efficiency**:
   - Given the module is running in a deterministic real-time loop,
   - When performing matrix operations or filter updates,
   - Then it executes within the allotted time slice and avoids dynamic memory allocation.

## Technical Notes
- The module should subscribe to sensor data topics on the internal communication bus.
- The module must be designed to be testable with mocked sensor inputs.
- Use fixed-point arithmetic if floating point performance is deemed a bottleneck on the target hardware (STM32F407).
- Ensure integration with the project's C++ policy (e.g., no raw dynamic allocations).
