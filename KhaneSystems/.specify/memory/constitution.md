<!--
Sync Impact Report
- Version change: 1.5.0 -> 1.6.0
- Modified principles:
  - Added driver architecture governance for IMU/Gyroscope/AirSpeed and related sensors
  - Added embedded C++ subset policy (exceptions/RTTI/allocation/noexcept/constexpr)
  - Clarified project primary programming language as C++
  - Expanded II. Hardware Access Through Wrappers with Hook-based hardware simulation
  - Expanded V. Verification Before Integration with hardware-hook simulation expectations
  - Expanded V. Verification Before Integration with Gazebo simulation validation
  - Expanded V. Verification Before Integration with third-party EKF/Eigen3 checks
  - Expanded I. Drone-Type Modularity to include PID Controller
  - Expanded IV. Safety-Critical Determinism with PID-specific constraints
  - Added I. Drone-Type Modularity
  - Added II. Hardware Access Through Wrappers
  - Added III. RTOS Abstraction At Compile Time
  - Added IV. Safety-Critical Determinism
  - Added V. Verification Before Integration
- Added sections:
  - Technical Constraints
  - Development Workflow
- Removed sections:
  - None
- Templates requiring updates:
  - .specify/templates/plan-template.md: updated
  - .specify/templates/spec-template.md: updated
  - .specify/templates/tasks-template.md: updated
- Deferred items:
  - None
-->

# KhaNeSystems Constitution

## Core Principles

### I. Drone-Type Modularity
All feature work MUST preserve strict modular boundaries between drone domains and common platform code. The modules `Autopilot`, `Flight Controller`, `PID Controller`, `Propulsion Controller`, `Telemetry`, `Logging`, `Storage`, `RF Communication`, and `Sensor Fusion` MUST remain independently testable and replaceable. Drone-specific logic for `Fixed-Wing` and `QuadCopter` MUST be explicit and MUST NOT leak through implicit coupling in shared layers.

### II. Hardware Access Through Wrappers
Application and domain modules MUST access hardware only through project wrapper interfaces. Direct calls from application logic to STM32 HAL, MCU registers, or vendor low-level APIs are prohibited unless explicitly justified in design documentation and accepted during review. Wrapper contracts MUST expose stable semantics for portability and safe refactoring.
Test and simulation paths MUST support Hook-based implementations of wrapper contracts so business logic can execute without physical hardware. Hook classes MUST preserve the same interface contracts and timing semantics expected by production wrappers.
All hardware drivers (IMU, Gyroscope, Air Speed, and other sensors/actuators) MUST access buses, GPIO, timing, and interrupts only via project C++ wrappers.

### III. RTOS Abstraction At Compile Time
RTOS selection MUST be resolved at compile time and routed through the abstraction layer (`RtosAbstract`) and backend wrappers. Business logic MUST stay RTOS-agnostic and MUST NOT directly depend on ThreadX or FreeRTOS symbols. Build configuration MUST select one backend at a time and MUST avoid dual-linking RTOS implementations in the same firmware target.

### IV. Safety-Critical Determinism
Control-path code MUST prioritize deterministic behavior. Changes to control loops, scheduling, synchronization, and communication paths MUST avoid hidden dynamic allocation, unbounded blocking, and ambiguous ownership. Error handling in critical paths MUST be explicit, bounded, and reviewable.
For PID-based control laws, implementations MUST define explicit sampling period assumptions, actuator/output saturation behavior, anti-windup strategy, and bumpless transfer behavior for mode switches or setpoint changes.

### V. Verification Before Integration
Any change touching wrappers, hardware access, RTOS abstraction, timing behavior, or drone-domain logic MUST include verification evidence before integration. Evidence MUST include at least one of: successful targeted build, focused automated test, or static review with documented rationale. Missing verification blocks integration.
Any integration of third-party numerical/control libraries (including EKF implementations and linear algebra dependencies such as `Eigen3`) MUST include traceable version/source pinning, deterministic-build checks, and functional validation with representative sensor datasets or replay scenarios.
For flight-dynamics or autonomy-impacting changes, verification SHOULD include simulation evidence in `Gazebo Sim` with documented scenario setup, expected outcomes, and pass/fail criteria.
For hardware-dependent features, verification SHOULD include Hook-based hardware simulation tests that validate normal behavior, boundary conditions, and fault injection paths.

## Technical Constraints

- Primary implementation language is C++ (C++20 for embedded firmware targets).
- Primary target is STM32F407 embedded firmware in C++20.
- Embedded C++ subset policy for control-critical firmware paths:
  - Exceptions MUST be disabled or avoided in real-time/control-critical code paths.
  - RTTI SHOULD be disabled for embedded production targets unless explicitly justified.
  - Dynamic heap allocation in steady-state control loops MUST be avoided.
  - Public wrapper and control-path APIs SHOULD use `noexcept` where failure is not part of contract.
  - `constexpr` SHOULD be used for compile-time constants/configuration and MUST NOT be used to misrepresent runtime HAL interactions.
- CMake is the canonical build system and MUST remain the source of RTOS selection.
- The `Wrappers` layer MUST stay above `HardwareAccessLayer`.
- Supported RTOS backends are currently limited to FreeRTOS and ThreadX.
- Architecture MUST continue to support at least `Fixed-Wing` and `QuadCopter`.
- Sensor fusion stack MAY integrate EKF implementations from external sources, provided they are wrapped behind project-owned `Sensor Fusion` interfaces.
- `Eigen3` MAY be used for matrix/vector operations when compile-time options and usage patterns are configured for embedded determinism (no hidden heap allocation in real-time paths).
- `Gazebo Sim` is the reference simulation environment for system-level behavior validation before hardware flight testing.
- Hardware simulation architecture SHOULD use Hook classes implementing wrapper interfaces for GPIO, communication buses, timing sources, and sensor/actuator interactions used by target features.
- Driver architecture constraints:
  - Each driver SHOULD be split into transport-independent logic and transport binding through wrappers.
  - Driver public interfaces SHOULD expose typed data structures, timestamps, and explicit status/error codes.
  - Driver initialization MUST define deterministic startup sequence, calibration prerequisites, and timeout behavior.
  - Driver update/read paths in control-critical flows MUST be bounded and non-blocking by design contract.

## Development Workflow

- Every change proposal MUST identify impacted modules: `Autopilot`, `Flight Controller`, `PID Controller`, `Propulsion Controller`, `Telemetry`, `Logging`, `Storage`, `RF Communication`, `Sensor Fusion`, and any common layer.
- Code review MUST include boundary checks across application, wrappers, hardware access, and RTOS abstraction.
- Any direct dependency on HAL, ThreadX, or FreeRTOS outside approved abstraction layers MUST include written justification in the spec/plan.
- Any direct use of third-party EKF code or `Eigen3` in control-critical paths MUST include explicit rationale in the spec/plan, plus boundaries showing where project wrappers isolate external APIs.
- Any deviation from the embedded C++ subset policy (exceptions/RTTI/heap usage in critical paths) MUST be explicitly justified in the spec/plan.
- Features with hardware dependencies SHOULD document which Hook classes are used, what they simulate, and how simulation hooks map to production wrapper behavior.
- Features touching IMU/Gyroscope/Air Speed or other hardware drivers MUST document:
  - wrapper interfaces used (I2C/SPI/UART/GPIO/Timer/etc.),
  - sampling/update rate assumptions,
  - fault handling (timeout, invalid sample, stale data, bus error),
  - emitted health/status signals consumed by upper layers.
- Feature specs MUST declare whether behavior is `Fixed-Wing`, `QuadCopter`, or shared/common.
- Features touching `PID Controller` MUST document gains/tuning source, saturation limits, anti-windup method, and validation scenario(s) representative of expected flight envelope.
- Features touching `Sensor Fusion` MUST document EKF state definition, process/measurement models, covariance assumptions, and fallback behavior on sensor fault or divergence detection.
- Features affecting dynamics, control, navigation, or mission autonomy SHOULD define at least one `Gazebo Sim` validation scenario, including initial conditions, disturbances, and success thresholds.
- Task breakdowns SHOULD separate work by layer: domain module, wrapper/API, hardware/RTOS integration, and verification.

## Governance

This constitution overrides local implementation habits when conflicts arise.

Amendment process:
1. Propose constitution updates in a tracked change with rationale and impact.
2. Review impact on templates and active workflows before merge.
3. Update related templates and guidance files in the same change whenever possible.

Versioning policy:
- MAJOR: breaking governance changes or principle removal/redefinition.
- MINOR: new principle/section or materially expanded requirements.
- PATCH: clarifications, wording improvements, or typo-level updates.

Compliance expectations:
- Plan reviews MUST include an explicit constitution check.
- Spec and task artifacts MUST remain aligned with this document.
- Non-compliant changes require documented exception approval.

**Version**: 1.6.0 | **Ratified**: 2026-05-06 | **Last Amended**: 2026-05-07
