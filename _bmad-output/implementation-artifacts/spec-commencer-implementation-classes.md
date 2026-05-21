---
title: 'Commencer implementation classes'
type: 'feature'
created: '2026-05-21'
status: 'in-progress'
context:
  - '{project-root}/_bmad-output/planning-artifacts/architecture.md'
---

<frozen-after-approval reason="human-owned intent - do not modify unless human renegotiates">

## Intent

**Problem:** Les classes de base propulsion, controle de vol et fusion capteurs sont encore partiellement stubbees ou incoherentes avec les tests, ce qui bloque le demarrage d'une implementation executable.

**Approach:** Implementer une premiere couche deterministe, sans allocation dynamique, avec etat observable et validations minimales pour que les modules puissent etre testes sur host.

## Boundaries & Constraints

**Always:** Conserver C++20, APIs publiques `noexcept` quand possible, pas d'exceptions, pas d'allocation dynamique, pas de dependance runtime externe.

**Ask First:** Tout changement de modele physique avance, integration EKF reelle, PWM hardware, ou refonte des namespaces publics.

**Never:** Ne pas toucher aux suppressions/migrations Git existantes, ne pas ajouter de HAL STM32 dans les tests host, ne pas melanger cette tranche avec navigation ou telemetry.

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| Propulsion locked | state machine non autorisee | thrust force a 0, retour false | no throw |
| Low battery | propulsion autorisee, voltage < seuil | thrust limite a 0.5 | clamp deterministic |
| Invalid IMU | pointeur nul, NaN, acceleration hors borne | etat precedent conserve | update ignore |
| Flight command | throttle/pitch/roll/yaw | commande stockee dans la dynamique choisie | clamp throttle |

</frozen-after-approval>

## Code Map

- `khane_systems/Propulsion/FlightController.hpp` -- controle generique quad/fixed-wing.
- `khane_systems/Propulsion/propulsion_controller.hpp` -- verrouillage propulsion et limitation batterie.
- `khane_systems/sensor_fusion/SensorFusion.hpp` -- facade `ISensorFusion`.
- `khane_systems/sensor_fusion/SensorFusion.cpp` -- validation IMU et etat attitude/position.
- `tests/unit/*` -- tests host existants pour les classes visees.

## Tasks & Acceptance

**Execution:**
- [x] `khane_systems/Propulsion/FlightController.hpp` -- rendre les dynamiques observables et deterministic.
- [x] `khane_systems/Propulsion/propulsion_controller.hpp` -- corriger includes/clamping et API de lecture.
- [x] `khane_systems/sensor_fusion/*` -- ajouter constructeur par defaut, validation pointeurs/NaN, mise a jour simple.
- [x] `khane_systems/*/CMakeLists.txt` -- compiler les vraies sources necessaires.
- [x] `tests/unit/*` -- aligner tests host sur les APIs reelles.

**Acceptance Criteria:**
- Given a locked state machine, when thrust is applied, then current thrust is zero and the command is rejected.
- Given nominal IMU acceleration, when sensors are updated, then attitude remains finite and readable.
- Given a selected flight dynamics type, when update runs, then the last command is retained by that dynamics instance.

## Verification

**Commands:**
- `cmake --build build-test` -- expected: modules compile.
- `ctest --test-dir build-test --output-on-failure` -- expected: targeted tests pass when registered.
