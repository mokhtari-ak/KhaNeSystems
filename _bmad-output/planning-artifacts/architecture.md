---
stepsCompleted: [1, 2, 3, 4, 5, 6, 7, 8]
workflowType: 'architecture'
lastStep: 8
status: 'complete'
completedAt: '2026-05-18'
inputDocuments:
  - "_bmad-output/planning-artifacts/epics.md"
  - "_bmad-output/brainstorming/brainstorming-session-20260517-0820.md"
project_name: 'KhaNeSystems'
user_name: 'Karim'
date: '2026-05-18'
---

# Architecture Decision Document

_This document builds collaboratively through step-by-step discovery. Sections are appended as we work through each architectural decision together._

## Project Context Analysis

### Requirements Overview

**Functional Requirements:**
Le système doit supporter des drones multi-rotors et à voilure fixe sur STM32F407, avec une architecture modulaire et déterministe. Les fonctions clés incluent un bus d'événements typé, une fusion de capteurs par EKF, une machine à états de vol sécurisée, et un système de logging non-bloquant sur SD card (secteurs bruts).

**Non-Functional Requirements:**
Utilisation de C++20 (sous-ensemble critique), sans exceptions ni RTTI. Le déterminisme est impératif : pas d'allocation dynamique en vol, ISR minimales, et fréquences de boucle garanties. Le système doit être agnostique du RTOS et supporter la simulation HITL.

**Scale & Complexity:**
- Primary domain: Firmware / Embedded Systems (Flight Control)
- Complexity level: High (Critical real-time constraints, complex math/EKF, safety-critical)
- Estimated architectural components: ~12 (Drivers, Bus, Supervisor, SensorFusion, FlightControl, Propulsion, Logging, Params, Navigation, Telemetry, Power, BSP)

### Technical Constraints & Dependencies

- Matériel : STM32F407VG / AirSim (HITL)
- Bibliothèques : Eigen3 (matrices), haowanghk/kalman-filter-library (EKF)
- Toolchain : Arm GNU Toolchain (C++20)
- RTOS : Compatible FreeRTOS et AzureRTOS/ThreadX

### Cross-Cutting Concerns Identified

- Déterminisme temporel et synchronisation inter-tâches (ISRs/Bus)
- Gestion de la sécurité (Failsafe multiniveau et verrouillage propulsion)
- Intégrité numérique (Validation NaN/Inf dans les calculs EKF)
- Observabilité (Logging haute fréquence et télémétrie)

## Starter Template Evaluation

### Primary Technology Domain

Firmware / Embedded Systems (Flight Control) basé sur STM32F407.

### Starter Options Considered

1. **STM32CubeMX with CMake (Current)**: Utilisation de l'outil ST pour la génération du BSP/HAL et des fichiers CMake de base. Avantage : Intégration native avec le matériel et facilité de mise à jour des périphériques.
2. **Custom Modern C++ Embedded Template**: Structure manuelle privilégiant le C++20 pur, masquant la HAL derrière des interfaces strictes.

### Selected Starter: STM32CubeMX + CMake Custom Structure

**Rationale for Selection:**
Le projet contient déjà un fichier `.ioc` et une structure CMake. Nous allons utiliser cette base comme "Starter" tout en y superposant une architecture C++20 moderne pour garantir le déterminisme et l'abstraction RTOS demandés.

**Initialization Command:**
Le projet est déjà initialisé. La prochaine étape consiste à structurer les dossiers selon les couches définies.

**Architectural Decisions Provided by Starter:**

**Language & Runtime:**
C++20 (Arm GNU Toolchain), options `-fno-exceptions -fno-rtti`.

**Build Tooling:**
CMake 3.20+ avec support des presets (`CMakePresets.json`) pour la cross-compilation.

**Testing Framework:**
Intégration de GoogleTest (ou Catch2) pour les tests unitaires sur machine hôte (via mocks des wrappers matériels).

**Code Organization:**
Structure en couches : `Drivers/` (HAL), `Libs/` (Abstractions/Bus), `KhaneSystems/` (Logique de vol/EKF).

**Development Experience:**
Utilisation de VS Code avec l'extension STM32 VS Code Extension ou CMake Tools.

## Core Architectural Decisions

### Decision Priority Analysis

**Critical Decisions (Block Implementation):**
- **Internal Communication**: Mediator Pattern statique (C++20) avec typage fort.
- **RTOS Abstraction**: Wrappers statiques (via namespaces/CMake) pour éliminer l'overhead des vtables.

**Important Decisions (Shape Architecture):**
- **Data Buffering**: Modèle Ouroboros (circular buffers) avec politique "Overwrite Oldest" pour les données temps-réel.
- **Failsafe Management**: Machine à États Hiérarchique (HSM) pour une gestion sécurisée et graduée des pannes.

**Deferred Decisions (Post-MVP):**
- **Adaptive EKF Order**: La logique exacte de réduction d'ordre de l'EKF sera affinée après les premiers tests de charge CPU (Story 4.4).

### Data Architecture & Communication

- **Pattern**: Mediator Statique.
- **Rationale**: Déterminisme garanti, pas d'allocation dynamique, typage vérifié à la compilation.
- **Buffers**: Ring buffers à taille fixe (`constexpr`) par Topic.

### RTOS Abstraction

- **Approach**: Static Wrappers.
- **Backends**: FreeRTOS et AzureRTOS (ThreadX).
- **Selection**: Configuré via CMake (source sets commutables).
- **Rationale**: Performance maximale pour l'embarqué critique (zéro indirection).

### Security & Failsafe

- **Mechanism**: Hierarchical State Machine (HSM).
- **Triggers**: Monitoring Heartbeat, Watchdog matériel, et seuils de batterie/capteurs.
- **Locking**: Verrouillage logiciel strict de la propulsion hors état ARMÉ.

### Decision Impact Analysis

**Implementation Sequence:**
1. Implémentation du RTOS Wrapper (Base).
2. Développement du Bus d'événements statique (Communication).
3. Mise en place de la Machine à États de base (Superviseur).
4. Intégration des couches applicatives (EKF, Flight Control).

**Cross-Component Dependencies:**
Le Bus d'événements est le socle de tous les autres modules. Le Superviseur (HSM) dépend du Bus pour recevoir les alertes et envoyer les commandes de Failsafe.

## Implementation Patterns & Consistency Rules

### Naming Patterns

**Code Naming Conventions:**
- **Classes/Structs**: `snake_case` (ex: `class sensor_manager`).
- **Methods/Functions**: `snake_case` (ex: `void read_all()`).
- **Variables**: `snake_case` (ex: `uint32_t sample_count`).
- **Constants/Macros**: `SCREAMING_SNAKE_CASE` (ex: `MAX_BUFFER_SIZE`).
- **Namespaces**: `snake_case` (ex: `namespace khane::bus`).

**File Naming Conventions:**
- **Files**: `snake_case` avec extensions `.hpp` pour les headers et `.cpp` pour les sources (ex: `imu_wrapper.hpp`).

### Structure Patterns

**Project Organization:**
- **Module Structure**: Dossiers "Flat" par module. Chaque dossier contient les headers, les sources et les tests unitaires du module.
- **Location**:
  - `Libs/` : Abstractions, Bus, RTOS Wrappers.
  - `KhaneSystems/` : Logique de vol, EKF, Navigation.
  - `Drivers/` : Code généré par STM32CubeMX.

**Test Organization:**
- Les fichiers de test doivent être nommés `[module_name]_test.cpp` et situés dans le même dossier que le module.

### Format Patterns

**API & Communication:**
- **Topics**: `enum class topic_id : uint8_t { imu_data, battery_status, ... }`.
- **Payloads**: `struct` simples (POD) sans méthodes complexes.

**Error Handling:**
- Utilisation systématique de `noexcept` sur les APIs publiques.
- Retour d'erreur via `std::optional` ou codes d'erreur typés (enum), jamais d'exceptions.

### Communication Patterns

**Event System:**
- **Pattern**: Mediator Statique.
- **Payloads**: Taille fixe garantie à la compilation.
- **Buffers**: Écrasement du plus vieux (`Overwrite Oldest`) pour les données temps-réel.

### Enforcement Guidelines

**All AI Agents MUST:**
- Suivre strictement le style `snake_case`.
- Ne jamais utiliser `new` ou `malloc` après l'initialisation.
- Documenter chaque interface publique avec des commentaires Doxygen simplifiés.

## Project Structure & Boundaries

### Complete Project Directory Structure

```text
KhaNeSystems/
├── Libs/
│   ├── common/                # Shared utilities (circular buffers, etc.)
│   ├── rtos_abstract/         # RTOS Static Wrappers
│   └── internal_communication/ # Static Mediator Event Bus
├── KhaneSystems/              # Core Application Logic
│   ├── supervisor/            # HSM-based System Monitor & Failsafe
│   ├── sensor_fusion/         # EKF-based state estimation
│   ├── flight_controller/     # Control loops (PID)
│   ├── propulsion/            # Motor/Servo output logic
│   ├── navigation/            # Waypoint & Mission execution
│   └── power_monitor/         # Battery & Energy management
├── Modules/
│   ├── eigen/                 # Matrix library
│   └── kalman_filter_library/  # EKF implementation
└── Tests/
    └── unit/                  # Host-based unit tests
```

### Architectural Boundaries

**API Boundaries:**
L'application (`KhaneSystems/`) communique exclusivement via le `rtos_abstract` pour les services système et via l' `internal_communication` pour les données entre modules.

**Data Boundaries:**
Les accès à la Flash et à la carte SD (Logging) sont centralisés dans des services dédiés dans `Libs/common/` pour garantir le déterminisme.

### Requirements to Structure Mapping

**Feature/Epic Mapping:**
- **Event Bus (Epic 1)** : `Libs/internal_communication/`
- **Supervisor & Failsafe (Epic 2)** : `KhaneSystems/supervisor/`
- **Propulsion Safety (Epic 3)** : `KhaneSystems/propulsion/`
- **EKF & Sensor Validation (Epic 4)** : `KhaneSystems/sensor_fusion/`
- **Navigation (Epic 5)** : `KhaneSystems/navigation/`

### Integration Points

**Internal Communication:**
Utilisation du Mediator Pattern statique. Chaque module s'abonne aux topics dont il a besoin au démarrage.

**Data Flow:**
Flux unidirectionnel : Capteurs -> Bus -> Fusion -> Bus -> Contrôle -> Propulsion.

## Architecture Validation Results

### Coherence Validation ✅

**Decision Compatibility:**
Toutes les décisions techniques (C++20, Eigen3, Mediator Statique, Wrappers RTOS) sont compatibles et optimisées pour le STM32F407.

**Pattern Consistency:**
Les patterns `snake_case` et l'organisation modulaire sont appliqués uniformément à tous les composants.

**Structure Alignment:**
La structure du projet reflète fidèlement les couches d'abstraction et les frontières de sécurité.

### Requirements Coverage Validation ✅

**Epic/Feature Coverage:**
Chaque Epic a un emplacement dédié et une stratégie d'implémentation documentée.

**Functional Requirements Coverage:**
Les 20 FRs sont couvertes, avec une attention particulière sur le déterminisme et la sécurité.

**Non-Functional Requirements Coverage:**
Les contraintes critiques (pas d'allocation dynamique, pas d'exceptions) sont imposées comme règles de cohérence.

### Implementation Readiness Validation ✅

**Decision Completeness:**
Toutes les décisions critiques (Communication, RTOS, Sécurité) sont actées.

**Structure Completeness:**
L'arborescence complète est définie, ne laissant aucune place à l'improvisation pour les agents IA.

**Pattern Completeness:**
Les règles de nommage et de communication sont exhaustives.

### Gap Analysis Results

**Gaps (Priorité Basse) :**
- La configuration précise des fréquences de boucle (Attitude vs Navigation) devra être spécifiée lors de l'implémentation de la `flight_control`.
- La documentation Doxygen devra être générée systématiquement.

### Architecture Completeness Checklist

**Requirements Analysis**
- [x] Project context thoroughly analyzed
- [x] Scale and complexity assessed
- [x] Technical constraints identified
- [x] Cross-cutting concerns mapped

**Architectural Decisions**
- [x] Critical decisions documented with versions
- [x] Technology stack fully specified
- [x] Integration patterns defined
- [x] Performance considerations addressed

**Implementation Patterns**
- [x] Naming conventions established
- [x] Structure patterns defined
- [x] Communication patterns specified
- [x] Process patterns documented

**Project Structure**
- [x] Complete directory structure defined
- [x] Component boundaries established
- [x] Integration points mapped
- [x] Requirements to structure mapping complete

### Architecture Readiness Assessment

**Overall Status:** READY FOR IMPLEMENTATION

**Confidence Level:** High

**Key Strengths:**
Déterminisme garanti par construction, modularité forte via le bus statique, et abstraction RTOS "zero-cost".

### Implementation Handoff

**AI Agent Guidelines:**
- Follow all architectural decisions exactly as documented
- Use implementation patterns consistently across all components
- Respect project structure and boundaries
- Refer to this document for all architectural questions

**First Implementation Priority:**
Initialisation des `rtos_abstract` wrappers et du `internal_communication` bus.
