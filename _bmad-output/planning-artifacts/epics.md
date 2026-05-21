---
stepsCompleted: [1, 2]
---
## Requirements Inventory

### Functional Requirements

FR1: Le système doit supporter des drones multi-rotors et à voilure fixe sur STM32F407.
FR2: Architecture modulaire, déterministe et zero-allocation.
FR3: Bus d'événements typé pour la communication interne (Mediator Pattern).
FR4: Fusion de capteurs par EKF (via `haowanghk/kalman-filter-library`).
FR5: Machine à états de vol sécurisée (DISARMED, ARMED, FAILSAFE, CALIBRATING).
FR6: Logging non-bloquant sur SD card (secteurs bruts).
FR7: Gestion centralisée des paramètres avec validation de bornes.
FR8: Superviseur système avec Heartbeat et Watchdog.
FR9: Verrouillage propulsion sécurisé (Safety Lock).
FR10: Monitoring batterie et télémétrie de puissance.

### Non-Functional Requirements

NFR1: Utilisation de C++20 (sous-ensemble critique), sans exceptions ni RTTI.
NFR2: Déterminisme impératif : aucune allocation dynamique en vol.
NFR3: RTOS-agnostique (FreeRTOS / ThreadX via wrappers).
NFR4: Utilisation d'Eigen3 avec `EIGEN_NO_MALLOC`.
NFR5: APIs publiques marquées `noexcept`.

### Additional Requirements

- Déterminisme temporel et synchronisation inter-tâches (ISRs/Bus).
- Intégrité numérique (Validation NaN/Inf dans les calculs EKF).
- Observabilité (Logging haute fréquence et télémétrie).
- Simulation support (HITL via AirSim).

## Requirements Coverage Map

| FR | Epic |
| :--- | :--- |
| FR1 | Epic 1 |
| FR2 | Epic 1 |
| FR3 | Epic 1 |
| FR4 | Epic 1 |
| FR5 | Epic 2 |
| FR6 | Epic 2 |
| FR7 | Epic 4 |
| FR8 | Epic 2 |
| FR9 | Epic 2 |
| FR10 | Epic 3 |

## Epics List

1. **Epic 1: Système de Vol Fondamental** (FR1, FR2, FR3, FR4) - Stabilisation de base, Drivers, EKF, Contrôle PID.
2. **Epic 2: Sécurité Active & Supervision** (FR5, FR6, FR8, FR9) - HSM, Failsafe, Watchdog.
3. **Epic 3: Gestion Énergie & Monitoring** (FR10) - PowerMonitor, batterie.
4. **Epic 4: Connectivité & Paramétrage** (FR7) - MAVLink, Parameter Server.
