# Story 2.4: Séquence de démarrage (Boot) sécurisée

## Description
As a pilot,
I want a strictly ordered and validated startup sequence,
So that the drone only allows arming when the entire system is healthy.

## Acceptance Criteria
- **Given** drone power-on
- **When** the boot sequence executes
- **Then** each layer (BSP, Clock, Drivers, EKF) must validate successfully before proceeding to the next layer
- **And** if any layer fails validation, the boot sequence halts and transitions to a safe state
- **And** the ARMED state is strictly prohibited until the final initialization stage is successfully completed and verified
- **And** each successful initialization stage is logged with a timestamp and status to the non-blocking event bus
- **And** the System Supervisor monitors the progress of the startup sequence and triggers a FAILSAFE if the boot sequence times out
