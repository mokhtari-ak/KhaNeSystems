# KhaNeSystems Documentation Index

## Overview
KhaNeSystems is a modular, deterministic, and safety-critical firmware architecture for drones (multi-rotor and fixed-wing), targeted at the STM32F407 hardware platform.

## Architecture
- [Architecture Overview](architecture.md)

## Requirements
- [Functional & Non-Functional Requirements](epics.md)

## Modules
- [Storage](modules/storage.md)
- [Supervisor](modules/supervisor.md)
- [Propulsion](modules/propulsion.md)
- [Sensor Fusion](modules/sensor_fusion.md)
- [Navigation](modules/navigation.md)

## Development
- [Initialisation](implementation/1-0-initialisation-et-structure-du-projet.md)
- [Event Bus](implementation/1-1-bus-devenements-interne-event-bus-type-et-deterministe.md)
- [Logging](implementation/1-2-logging-non-bloquant-avec-buffer-circulaire-ouroboros.md)
- [Priorities & Overwrite](implementation/1-3-gestion-des-priorites-et-ecrasement-rate-limiting-overwrite.md)
- [Flight Controller](implementation/5-2-controleur-de-vol-commutable-quadcopter-vs-fixed-wing.md)
