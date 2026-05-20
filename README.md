## Mon Premier projet Embarqué
(Même ce fichier est généré par IA)
# 🚁 Drone Firmware Framework — STM32F407VG

Firmware framework temps réel déterministe pour drones **Fixed-Wing** et **Quadcopter**, développé en **C++20 embarqué critique** pour le microcontrôleur STM32F407VG.

Ce projet suit une approche :
- **BMAD Method** (*Behavior-Model Architecture-Driven Development*)
- **Specification-Driven Development (SDD)**
- développement assisté par **IA générative**
- architecture firmware modulaire orientée sûreté et déterminisme temps réel.

Le système est conçu pour :
- les drones critiques embarqués,
- les architectures RTOS déterministes,
- les environnements SIL/HITL,
- la simulation avancée sous AirSim,
- et l’évolution future vers des architectures avioniques avancées.

---

# ✨ Objectifs du Projet

- Architecture firmware modulaire et testable
- Compatibilité Fixed-Wing et Quadcopter
- Abstraction complète RTOS
- Déterminisme temps réel strict
- Safety/Failsafe robuste
- Simulation SIL/HITL
- Support DMA intensif
- Aucun code applicatif dépendant directement de la HAL STM32
- Développement guidé par spécifications (BMAD + IA)

---

# 🧠 Philosophie du Projet

Il s’agit d’une plateforme avionique embarquée inspirée de :
- PX4
- ArduPilot
- architectures DAL temps réel critiques.

L’objectif est de construire un système :
- déterministe,
- extensible,
- testable,
- portable,
- orienté sûreté,
- compatible avec des workflows IA modernes.

---

# 🧩 Méthodologie — BMAD + IA

Le projet suit une approche BMAD Method.

## Principes appliqués

- Développement piloté par architecture
- Génération guidée par spécifications
- Découpage strict des responsabilités
- Validation systématique des contraintes RT
- Documentation avant implémentation
- Génération de code assistée par IA
- Vérification architecturale continue

## Utilisation de l’IA

L’IA est utilisée pour :
- génération contrôlée de code,
- génération d’architectures,
- validation des contraintes,
- génération des drivers,
- génération des wrappers RTOS,
- génération de tests,
- génération de documentation,
- vérification de cohérence.

Les prompts IA sont eux-mêmes considérés comme des artefacts d’architecture.

---

# ⚙️ Plateforme Matérielle

## MCU Principal

- MCU : STM32F407VG
- Architecture : ARM Cortex-M4F
- Fréquence : 168 MHz
- Flash : 1 MB
- SRAM : 192 KB
- FPU : simple précision uniquement

## Contraintes critiques

- Pas de cache instruction/données
- CCM inaccessible DMA
- DMA statiquement planifié
- Aucun heap runtime dans les chemins critiques
- Interruptions bornées
- Temps réel strict

---

# 🛠 Stack Technique

| Domaine | Technologie |
|---|---|
| Langage | C++20 embarqué |
| Build System | CMake |
| MCU | STM32F407VG |
| HAL | STM32Cube HAL encapsulée |
| RTOS | FreeRTOS / ThreadX |
| Math | Eigen3 |
| EKF | haowanghk/kalman-filter-library |
| Simulation | AirSim |
| Tests | GoogleTest |
| Static Analysis | clang-tidy / cppcheck |
| Documentation | Doxygen |

---

# 🧱 Architecture Générale

```text
Application Layer
│
├── FlightControl
├── Navigation
├── SensorFusion
├── Failsafe
├── PowerManagement
├── Telemetry
├── Logging
│
├── InternalCommunication
│     └── Typed EventBus
│
├── Drivers
│     ├── IMU
│     ├── GNSS
│     ├── Barometer
│     ├── Airspeed
│     ├── Magnetometer
│     ├── RC Input
│     └── Servo/ESC
│
├── RtosAbstract
│     ├── FreeRTOS Backend
│     └── ThreadX Backend
│
├── STM32F4HAL Wrappers
│
└── BSP (STM32CubeMX generated)
```

---

# 🔒 Contraintes Temps Réel

## Interdictions

- Exceptions C++
- RTTI
- Allocation dynamique runtime critique
- std::vector en RT
- std::string en RT
- std::function en RT
- printf dans les boucles critiques
- Appels HAL directs depuis la logique métier

## Exigences

- APIs `noexcept`
- DMA non bloquant
- Static allocation
- ISR minimales
- Budgets CPU documentés
- Deadlines vérifiées
- Heartbeats watchdog

---

# 🧵 RTOS Supportés

## FreeRTOS
- Static allocation uniquement
- `configSUPPORT_STATIC_ALLOCATION=1`
- `configSUPPORT_DYNAMIC_ALLOCATION=0`

## ThreadX
- Byte pools statiques
- Backend encapsulé

## Architecture RTOS Agnostique

La logique métier dépend uniquement de :

```cpp
rtos::Task<StackSize>
rtos::Queue<T, Depth>
rtos::Mutex
rtos::BinarySemaphore
rtos::HighResTimer
```

---

# 📡 Capteurs Supportés

| Driver | Interface |
|---|---|
| IMU | SPI DMA |
| Barometer | SPI |
| GNSS | UART DMA |
| Airspeed | I2C |
| Magnetometer | I2C |
| RC Input | UART DMA / Timer Capture |
| Battery Monitor | ADC DMA |

---

# 🧠 Fusion de Capteurs

Le système utilise :
- EKF encapsulé
- architecture SensorFusion indépendante
- validation capteurs stricte
- détection de divergence
- gestion fault-tolerant

---

# 🛡 Sécurité & FAILSAFE

## États principaux

```text
DISARMED
CALIBRATING
ARMED
FAILSAFE
```

## Déclencheurs FAILSAFE

- RC_LOST
- EKF divergence
- perte capteur
- surcharge CPU
- batterie critique
- watchdog timeout
- geofence breach
- airspeed stall (Fixed-Wing)

## Supervision

- watchdog matériel IWDG
- watchdog logiciel
- heartbeat par module
- FaultBus centralisé

---

# ✈️ Support Multi-Drone

## Fixed-Wing

- Airspeed control
- Stall detection
- Guidance
- TECS-ready
- Servos + ESC

## Quadcopter

- Mixing configurable
- Hover control
- ESC indépendants
- Altitude hold

La sélection est effectuée au compile-time :

```cpp
template<DroneType Type>
class FlightControlApp;
```

---

# 🧪 Simulation & Tests

## SIL
Simulation Linux avec hooks capteurs.

## HITL
Simulation temps réel avec AirSim.

## Tests

- Unit tests
- Regression tests
- Sensor fault injection
- EKF replay
- Safety response validation

---

# 📂 Structure du Projet

```text
/Core
/Application
/Drivers
/Modules
/RtosAbstract
/Libs
/BSP
/Tests
/Docs
/Tools
/CMake
```

---

# 🧰 Build

## Configuration

```bash
cmake -B build -DRTOS_TARGET=FreeRTOS
```

ou

```bash
cmake -B build -DRTOS_TARGET=ThreadX
```

## Compilation

```bash
cmake --build build
```

---

# 📊 Analyse Statique

Le pipeline CI/CD vérifie :
- stack usage,
- contraintes mémoire,
- placement DMA,
- heap interdit,
- règles clang-tidy,
- contraintes MISRA partielles,
- violations architecturales.

---

# 📚 Documentation

La documentation inclut :
- Architecture
- RTOS abstraction
- Drivers
- Safety
- Memory layout
- DMA mapping
- EventBus
- State Machines
- Timing constraints

---

# 🚀 Évolution Future

Objectifs futurs :
- MAVLink v2
- Mission Planner
- Vision Navigation
- Multi-vehicle
- Avoidance
- TECS avancé
- Navigation autonome
- Intégration future avec cFS / F´

---

# 🤝 Contribution

Les contributions doivent respecter :
- les contraintes temps réel,
- l’architecture BMAD,
- les règles de sûreté,
- les contraintes STM32F407,
- les règles C++20 embarquées,
- et les spécifications de déterminisme.

Toute contribution doit être :
- documentée,
- testée,
- analysée statiquement,
- et compatible SIL/HITL.

---

# 📄 Licence

À définir :
- MIT
- Apache 2.0
- GPLv3
- ou licence propriétaire.

---

# ⚠️ Avertissement

Ce projet cible des systèmes embarqués critiques.

Toute utilisation sur un drone réel nécessite :
- validation complète,
- essais SIL/HITL,
- validation hardware,
- revue sécurité,
- tests en environnement contrôlé.
