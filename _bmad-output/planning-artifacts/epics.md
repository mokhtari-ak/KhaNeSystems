---
stepsCompleted: [1, 2, 3, 4]
inputDocuments:
  - "User Prompt (Initial Exchange)"
  - "_bmad-output/brainstorming/brainstorming-session-20260517-0820.md"
  - "KhaneSystems/specs/001-hardware-abstraction-boundary/spec.md"
  - "KhaneSystems/specs/002-gazebo-quadcopter-ekf-pid/spec.md"
  - "KhaneSystems/specs/003-closed-loop-imu-ekf-pid-propulsion/spec.md"
  - "KhaneSystems/specs/004-flight-state-machine-base/spec.md"
---

# KhaNeSystems - Epic Breakdown

## Overview

This document provides the complete epic and story breakdown for KhaNeSystems, decomposing the requirements from the PRD and Architecture requirements into implementable stories.

## Requirements Inventory

### Functional Requirements

FR1: Support Fixed-wing and Quadcopter drone types.
FR2: Target STM32F407VG hardware.
FR3: Support AirSim (HITL) simulation via hooks.
FR4: Modular, deterministic, and secure firmware architecture.
FR5: RTOS agnostic (FreeRTOS/ThreadX) at compile-time.
FR6: Drivers for IMU, Barometer, GNSS, Air Speed via C++ wrappers.
FR7: Main state machine with DISARMED, ARMED, FAILSAFE states.
FR8: Propulsion strictly prohibited except in ARMED state.
FR9: State transitions must be logged (Reason and Origin).
FR10: EKF implementation from 'haowanghk/kalman-filter-library' encapsulated in "Sensor Fusion".
FR11: Internal communication via typed, versioned, and deterministic event bus.
FR12: Non-blocking logging with bounded circular buffers and levels (DEBUG, INFO, etc.).
FR13: System supervisor with heartbeat monitoring and watchdog.
FR14: Centralized parameter management with safe defaults and bounds validation.
FR15: Sensor validation (saturation, NaN, timeout detection).
FR16: Failsafe triggers (RF loss, battery low, sensor timeout, EKF divergence, etc.).
FR17: Compulsory startup sequence (BSP -> Clock -> HAL -> ... -> ARMED).
FR18: Battery monitoring and energy telemetry.
FR19: Non-blocking sensor reading/updating in critical flows.
FR20: Deterministic and bounded synchronization in ISRs.

### NonFunctional Requirements

NFR1: C++20 (embedded critical subset).
NFR2: Exceptions strictly forbidden.
NFR3: RTTI disabled in production.
NFR4: Dynamic allocation forbidden in steady-state control loops.
NFR5: Public APIs for wrappers and control paths must use 'noexcept'.
NFR6: Use Eigen3 for matrix/vector operations (deterministic config).
NFR7: ISRs must be minimal and non-blocking.
NFR8: Real-time loop frequencies explicitly defined (Attitude, Navigation, Telemetry, Logging).
NFR9: Monotonic system clock for all time measurements.
NFR10: Binary logging preferred for high frequency.
NFR11: Static polymorphism and compile-time configuration preferred over dynamic.

### Additional Requirements

- **RTOS Abstraction**: Agnostic backends for FreeRTOS and ThreadX.
- **Internal Communication (Arch #5, #6, #8)**: Rate-limited message bus, Ouroboros circular buffers with priority-based overwrite, and sequence number monitoring.
- **Logging & Storage (Arch #12, #13, Safety #3, #9)**: Delta-logging, double-buffer ping-pong for SD, black box snapshots, and raw sector storage.
- **Supervisor (Arch #16, #18, Safety #13)**: Heartbeat table, load shedding (throttling), and hardware dead-man switch.
- **Mathematical Safety (Arch #21)**: Adaptive EKF order based on CPU load.
- **Tooling (Tooling #1)**: Schema-aware log reconstructor for offline analysis.

### UX Design Requirements

*Aucun document UX fourni. Les exigences d'interface sont dérivées des besoins de télémétrie opérateur (FR9, FR12, FR18).*

### FR Coverage Map

FR1: Epic 5 - Support Fixed-wing and Quadcopter drone types.
FR2: Epic 5 - Target STM32F407VG hardware.
FR3: Epic 4/5 - Support AirSim (HITL) simulation via hooks.
FR4: All Epics - Modular, deterministic, and secure firmware architecture.
FR5: Epic 1 - RTOS agnostic (FreeRTOS/ThreadX) at compile-time.
FR6: Epic 4 - Drivers for IMU, Barometer, GNSS, Air Speed via C++ wrappers.
FR7: Epic 2 - Main state machine with DISARMED, ARMED, FAILSAFE states.
FR8: Epic 3 - Propulsion strictly prohibited except in ARMED state.
FR9: Epic 1 - State transitions must be logged (Reason and Origin).
FR10: Epic 4 - EKF implementation from 'haowanghk/kalman-filter-library' encapsulated in "Sensor Fusion".
FR11: Epic 1 - Internal communication via typed, versioned, and deterministic event bus.
FR12: Epic 1 - Non-blocking logging with bounded circular buffers and levels (DEBUG, INFO, etc.).
FR13: Epic 2 - System supervisor with heartbeat monitoring and watchdog.
FR14: Epic 5 - Centralized parameter management with safe defaults and bounds validation.
FR15: Epic 4 - Sensor validation (saturation, NaN, timeout detection).
FR16: Epic 2 - Failsafe triggers (RF loss, battery low, sensor timeout, EKF divergence, etc.).
FR17: Epic 2 - Compulsory startup sequence (BSP -> Clock -> HAL -> ... -> ARMED).
FR18: Epic 3 - Battery monitoring and energy telemetry.
FR19: Epic 4 - Non-blocking sensor reading/updating in critical flows.
FR20: Epic 1 - Deterministic and bounded synchronization in ISRs.

## Epic List

### Epic 1: Socle de Communication & Diagnostic (Event Bus & Logging)
Permettre aux modules d'échanger des données de manière déterministe et de tracer chaque événement critique sur la SD Card sans perturber le temps réel.
**FRs covered:** FR5, FR9, FR11, FR12, FR20.

## Epic 1: Socle de Communication & Diagnostic (Event Bus & Logging)

Permettre aux modules d'échanger des données de manière déterministe et de tracer chaque événement critique sur la SD Card sans perturber le temps réel.

### Story 1.0: Initialisation et Structure du Projet

As a lead developer,
I want to initialize the project structure with CMake and CubeMX,
So that all team members and agents have a consistent workspace following architectural layers.

**Acceptance Criteria:**

**Given** the project architecture document
**When** the initialization script or manual setup is executed
**Then** the directory structure (Libs/, KhaneSystems/, Drivers/) is created
**And** the CMakeLists.txt and CMakePresets.json are configured for the STM32F407 target
**And** the initial project compiles with empty stubs for the main modules.

### Story 1.1: Bus d'Événements interne (Event Bus) typé et déterministe

As a firmware developer,
I want a typed internal event bus with fixed-size queues,
So that modules can exchange data deterministically without dynamic allocation.

**Acceptance Criteria:**

**Given** a message type `IMUData` and a registered subscriber
**When** a module publishes an `IMUData` message to the bus
**Then** all subscribers receive the data via a zero-copy or minimal copy mechanism
**And** no dynamic memory allocation occurs during the exchange process.

### Story 1.2: Logging non bloquant avec Buffer Circulaire (Ouroboros)

As a flight control system,
I want to log events into bounded circular buffers in RAM,
So that high-priority tasks are never blocked by storage I/O latency.

**Acceptance Criteria:**

**Given** a high-priority flight control task
**When** it generates a log entry (e.g., level INFO)
**Then** the entry is immediately stored in a RAM circular buffer
**And** the task execution time is not affected by the underlying storage media latency.

### Story 1.3: Gestion des priorités et écrasement (Rate-Limiting & Overwrite)

As a safety supervisor,
I want the communication bus to implement rate-limiting and priority-based overwriting,
So that critical data always has space in the queues even during module flooding.

**Acceptance Criteria:**

**Given** a full message queue
**When** a new high-priority message is published
**Then** the oldest message in the queue is overwritten to accommodate the new one
**And** a "dropped message" counter is incremented for diagnostic purposes.

### Story 1.4: Stockage sécurisé sur Secteurs Bruts (Raw Sector Storage)

As a black-box system,
I want to write logs directly to SD card raw sectors,
So that data integrity is maximized and filesystem overhead/corruption is eliminated.

**Acceptance Criteria:**

**Given** a block of logs ready for archival
**When** the Storage module writes to the SD card
**Then** data is written sequentially to raw sectors without a filesystem layer
**And** data integrity is verified using a checksum per block.

### Epic 2: Surveillance & Sécurité Système (Supervisor & Failsafe)
Garantir l'intégrité du drone par une surveillance active (Heartbeat) et une gestion intelligente de la charge CPU, capable de déclencher un Failsafe en cas d'anomalie.
**FRs covered:** FR7, FR13, FR16, FR17.

## Epic 2: Surveillance & Sécurité Système (Supervisor & Failsafe)

Garantir l'intégrité du drone par une surveillance active (Heartbeat) et une gestion intelligente de la charge CPU, capable de déclencher un Failsafe en cas d'anomalie.

### Story 2.1: Superviseur de Santé (Heartbeat Monitor)

As a system administrator,
I want a supervisor module to monitor the heartbeat of all critical modules,
So that I can detect and react to module hangs or crashes.

**Acceptance Criteria:**

**Given** a critical module (e.g., Sensor Task)
**When** the module fails to publish its heartbeat within its specific timeout window
**Then** the Supervisor detects the timeout and marks the module as failed
**And** a global system health alert is triggered via the event bus.

### Story 2.2: Gestionnaire de Failsafe multiniveaux

As a safety officer,
I want a multi-level failsafe manager to execute graduated safety actions,
So that the drone reacts appropriately to different types of failures.

**Acceptance Criteria:**

**Given** a critical failure (e.g., RC signal loss)
**When** the Supervisor requests a transition to the FAILSAFE state
**Then** the main state machine switches state immediately
**And** the propulsion system executes the predefined safety behavior (e.g., controlled descent for quadcopters).

### Story 2.3: Délestage de Charge CPU (Throttling)

As a flight controller,
I want the supervisor to throttle non-critical tasks during CPU spikes,
So that my high-priority control loops remain deterministic.

**Acceptance Criteria:**

**Given** a measured CPU load exceeding 85%
**When** the Supervisor activates the load shedding policy
**Then** non-critical tasks (Telemetry, Logging) reduce their execution frequency
**And** the Flight Control task maintains its nominal frequency and priority.

### Story 2.4: Séquence de démarrage (Boot) sécurisée

As a pilot,
I want a strictly ordered and validated startup sequence,
So that the drone only allows arming when the entire system is healthy.

**Acceptance Criteria:**

**Given** drone power-on
**When** the boot sequence executes
**Then** each stage (BSP, Clock, Drivers, EKF) must validate successfully before proceeding to the next
**And** the ARMED state is strictly prohibited until the final stage is successfully reached.

### Epic 3: Pilotage de l'Énergie & Propulsion sécurisée

Gérer la puissance électrique, surveiller la batterie et s'assurer que les moteurs ne s'activent qu'en état ARMÉ, avec une protection contre les chutes de tension.
**FRs covered:** FR8, FR18.

## Epic 3: Pilotage de l'Énergie & Propulsion sécurisée

Gérer la puissance électrique, surveiller la batterie et s'assurer que les moteurs ne s'activent qu'en état ARMÉ, avec une protection contre les chutes de tension.

### Story 3.1: Surveillance de la Batterie (Battery Monitor)

As a pilot,
I want to monitor real-time battery voltage and current,
So that I can receive low battery alerts before a critical failure.

**Acceptance Criteria:**

**Given** an ADC signal from the battery shunt
**When** the BatteryMonitor module updates its state
**Then** real-time voltage (V) and current (A) are published to the telemetry bus
**And** a "Battery Critical" status is raised if the voltage drops below a constexpr threshold.

### Story 3.2: Verrouillage logiciel de la Propulsion (Safety Lock)

As a safety officer,
I want the propulsion system to be software-locked outside of the ARMED state,
So that accidental motor activation is physically impossible during handling.

**Acceptance Criteria:**

**Given** the system is in DISARMED or FAILSAFE state
**When** a thrust command is sent to the PropulsionController
**Then** the command is ignored and output is forced to 0.0f
**And** a security event is logged if an illegitimate thrust attempt is detected.

### Story 3.3: Compensation de chute de tension (Voltage Sag Protection)

As a flight controller,
I want the system to limit peak current when battery voltage is low,
So that I can prevent CPU brownouts during aggressive maneuvers.

**Acceptance Criteria:**

**Given** a low battery state and a sudden thrust increase
**When** the measured voltage drops towards the critical brownout threshold
**Then** the PropulsionController limits the thrust slew rate to cap current draw
**And** priority is given to maintaining stable voltage for the onboard electronics.

### Epic 4: Fusion de Capteurs & Intégrité Mathématique (Sensor Fusion)
Fournir une estimation d'état (Position/Attitude) fiable via l'EKF, protégée contre les erreurs de calcul (NaN) et validant chaque capteur.
**FRs covered:** FR3, FR6, FR10, FR15, FR19.

## Epic 4: Fusion de Capteurs & Intégrité Mathématique (Sensor Fusion)

Fournir une estimation d'état (Position/Attitude) fiable via l'EKF, protégée contre les erreurs de calcul (NaN) et validant chaque capteur.

### Story 4.1: Interface Sensor Fusion & Encapsulation EKF

As an autopilot developer,
I want a clean high-level interface for sensor fusion that hides EKF complexity,
So that I can interchange estimation algorithms without impacting flight control logic.

**Acceptance Criteria:**

**Given** synchronized IMU and magnetometer measurements
**When** the SensorFusion module processes the data using Eigen3-based EKF
**Then** it outputs a stable attitude estimate (quaternion and Euler angles)
**And** the internal implementation details of 'KalmanFilterLibrary' are completely encapsulated.

### Story 4.2: Barrière Anti-NaN & Validation SIMD

As a safety engineer,
I want systematic validation of matrix operation results,
So that numerical divergences (NaN/Inf) are detected before they corrupt flight control.

**Acceptance Criteria:**

**Given** an EKF covariance or state calculation
**When** a NaN or Inf value is produced by an Eigen3 operation
**Then** the noexcept wrapper detects the anomaly before the data is published
**And** the system executes a "Soft-Reset" of the filter or transitions to FAILSAFE.

### Story 4.3: Validation des Capteurs (Sanity Checks)

As a data integrity monitor,
I want to validate sensor data consistency before fusion,
So that the EKF is not "poisoned" by erroneous or jittery measurements.

**Acceptance Criteria:**

**Given** a GNSS sample with high HDOP (> 2.0) or an IMU sample showing physical inconsistency
**When** the driver publishes the data to the event bus
**Then** the SensorFusion module marks the measurement as "Invalid" for this cycle
**And** a low-confidence warning is emitted via the diagnostic bus.

### Story 4.4: EKF adaptatif selon la charge CPU

As a system architect,
I want the EKF to support a reduced-order mode,
So that the drone remains stable even during extreme CPU load shedding.

**Acceptance Criteria:**

**Given** a CPU load-shedding command from the Supervisor
**When** the adaptive policy is active
**Then** the EKF switches from a full navigation model (15+ states) to a reduced attitude model (6 states)
**And** execution time is significantly reduced while maintaining basic flight stability.

### Epic 5: Navigation & Paramétrage Dynamique
Exécuter des missions de vol (Quadcopter/Fixed-Wing) en utilisant des paramètres centralisés et validés, avec une configuration de mission flexible.
**FRs covered:** FR1, FR2, FR14.

## Epic 5: Navigation & Paramétrage Dynamique

Exécuter des missions de vol (Quadcopter/Fixed-Wing) en utilisant des paramètres centralisés et validés, avec une configuration de mission flexible. La télémétrie vers la station au sol (GCS) utilisera le protocole **MAVLink v2** pour une compatibilité standard.

### Story 5.1: Gestionnaire de Paramètres centralisé (Parameter Server)

As a flight engineer,
I want a centralized parameter server with bounds validation,
So that I can safely tune flight performance without risking unstable configurations.

**Acceptance Criteria:**

**Given** a PID gain parameter for roll control
**When** the system boots or a parameter update is requested
**Then** the value is validated against predefined constexpr safety bounds
**And** the server ensures persistence across reboots using internal Flash or SD storage.

### Story 5.2: Contrôleur de Vol commutable (Quadcopter vs Fixed-Wing)

As a multi-platform developer,
I want to select the vehicle control logic at compile-time,
So that the binary is optimized and specific to the target drone dynamics.

**Acceptance Criteria:**

**Given** a specific vehicle type selection in CMake (e.g., Quadcopter)
**When** the firmware is compiled
**Then** only the relevant control laws and policies are included in the binary (static polymorphism)
**And** the navigation commands correctly adapt to the chosen vehicle's flight envelope.

### Story 5.3: Exécuteur de Mission par Waypoints

As a mission operator,
I want the drone to autonomously navigate through GPS waypoints,
So that complex flight paths can be executed without manual stick input.

**Acceptance Criteria:**

**Given** a list of valid GPS waypoints and an ARMED state
**When** the mission execution is triggered
**Then** the Navigation module calculates heading and altitude setpoints to reach each waypoint sequentially
**And** the mission is aborted to FAILSAFE if any waypoint is detected as unreachable given current battery levels.

