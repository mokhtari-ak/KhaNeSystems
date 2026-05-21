# Story 4.2: Barrière Anti-NaN Validation SIMD

As an Autopilot Developer,
I want a high-performance validation layer that intercepts Eigen3 operation outputs to detect NaN/Inf values,
So that I can ensure the EKF state remains robust and immediately trigger a safe state if corruption is detected.

## Description
This story introduces a validation "barrier" for the Sensor Fusion/EKF pipeline. Given that the EKF relies on high-speed matrix calculations using Eigen3, numerical instability (resulting in NaN or Inf) can lead to catastrophic failure. This validation layer will leverage SIMD-friendly techniques to scan output matrices or vectors for invalid floating-point values at every EKF cycle. If corruption is found, the system must trigger an immediate fail-safe (e.g., reset, emergency landing, or propulsion shutdown) to prevent uncontrolled flight.

## Acceptance Criteria

1. **Validation Barrier Implementation**:
   - Given the completion of an EKF cycle,
   - When the `SensorFusion` module outputs a state vector or covariance matrix,
   - Then the validation barrier iterates through the data to check for non-finite values (NaN, Inf).

2. **Performance Efficiency**:
   - Given the strict real-time requirements of the EKF loop,
   - When checking for invalid values,
   - Then the implementation uses SIMD intrinsics (or compiler-optimized equivalents) to maximize speed and minimize the impact on the EKF cycle time.

3. **Immediate Failsafe Trigger**:
   - Given the validator detects a NaN or Inf value,
   - When the integrity check fails,
   - Then the system immediately triggers the `FailsafeManager` (defined in Story 2.2) to initiate a safe state (e.g., emergency stop or system reset).

4. **Detailed Logging**:
   - Given a validation failure,
   - When the safe state is triggered,
   - Then the system logs the state vector/matrix where the corruption occurred, providing enough context to debug the source of instability.

5. **Testability**:
   - Given the validation layer,
   - When running unit tests with injected corrupted (NaN/Inf) input data,
   - Then the validation barrier correctly flags the input and asserts the failure.

## Technical Notes
- The validation should be integrated seamlessly into the `SensorFusion` pipeline, likely as a decorator or a post-processing step for Eigen3 outputs.
- Adhere to the project's embedded C++ policy: no exceptions, no dynamic memory allocation.
- Use `std::isfinite` or equivalent architecture-specific instructions (e.g., `vmaxnm` on ARM, if applicable) for checking numerical validity.
- The check MUST be deterministic and occur within every EKF cycle.
