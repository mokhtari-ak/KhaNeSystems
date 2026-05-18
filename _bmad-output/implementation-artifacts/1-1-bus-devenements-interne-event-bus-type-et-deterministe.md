# Story 1.1: Bus d'Événements interne (Event Bus) typé et déterministe

Status: ready-for-dev

<!-- Note: Validation is optional. Run validate-create-story for quality check before dev-story. -->

## Story

As a firmware developer,
I want a typed internal event bus with fixed-size queues,
So that modules can exchange data deterministically without dynamic allocation.

## Acceptance Criteria

1. **Given** a message type `IMUData` and a registered subscriber, **When** a module publishes an `IMUData` message to the bus, **Then** all subscribers receive the data via a zero-copy or minimal copy mechanism.
2. **Given** any message exchange on the event bus, **Then** no dynamic memory allocation occurs during the process.
3. **Given** a full message queue, **When** a new high-priority message is published, **Then** the oldest message in the queue is overwritten to accommodate the new one (Ouroboros policy).
4. **Given** a message exchange, **Then** the implementation must use C++20 features (concepts, templates) and be `noexcept`.

## Tasks / Subtasks

- [ ] Task 1: Définir les interfaces de base de l'Event Bus (AC: 1, 2, 4)
  - [ ] Créer `Libs/InternalCommunication/Inc/EventBus.hpp`
  - [ ] Implémenter le concept `EventMessage` pour valider les types de messages à la compilation.
- [ ] Task 2: Implémenter le buffer circulaire déterministe (Ouroboros) (AC: 3)
  - [ ] Créer `Libs/InternalCommunication/Inc/OuroborosBuffer.hpp`
  - [ ] Implémenter une structure de données `StaticQueue` avec politique d'écrasement.
- [ ] Task 3: Implémenter le mécanisme de Publication/Abonnement (AC: 1)
  - [ ] Développer la classe `Publisher` et `Subscriber`.
  - [ ] Assurer le support multi-abonnés sans allocation dynamique.
- [ ] Task 4: Intégrer avec RtosAbstract pour la synchronisation ISR (AC: 4)
  - [ ] Utiliser des types atomiques C++20 pour les index de buffer si nécessaire pour le lock-free.
- [ ] Task 5: Tests unitaires et validation (AC: 1, 2, 3)
  - [ ] Créer un scénario de test avec `IMUData` simulé.
  - [ ] Vérifier l'absence d'allocation dynamique via `EIGEN_NO_MALLOC` (ou équivalent pour l'Event Bus).

## Dev Notes

### Architecture Patterns & Constraints
- **C++20 Embedded Subset**: Utilisation intensive de `concepts` pour la validation statique et `templates` pour l'injection de dépendances.
- **Zero Dynamic Allocation**: Toutes les files d'attente doivent être allouées statiquement au démarrage (taille fixée par template).
- **Noexcept Policy**: Toutes les fonctions de publication/lecture doivent être marquées `noexcept`.
- **Ouroboros Overwrite**: En cas de file pleine, écraser systématiquement la donnée la plus ancienne.

### Source Tree Components
- `Libs/InternalCommunication/` : Nouveau module pour la communication inter-tâches.
- `Libs/Common/SensorTypes.hpp` : Contient `IMUData` à utiliser comme type de message de référence.
- `Libs/RtosAbstract/` : Pour la synchronisation éventuelle via `StaticMutex`.

### Testing Standards
- Tests unitaires requis dans `Tests/Unit/InternalCommunication/`.
- Validation HITL via AirSim Hooks à prévoir dans une story ultérieure.

## Project Structure Notes
- Alignment with `Libs/` architecture pattern established during initial design.
- Namespace usage: `Communication::EventBus`.

### References
- [Source: _bmad-output/brainstorming/brainstorming-session-20260517-0820.md#Arch #5, #6, #8]
- [Source: Libs/Common/SensorTypes.hpp]

## Dev Agent Record

### Agent Model Used

Gemini 2.0 Flash

### Debug Log References

### Completion Notes List
- Initial context engine analysis completed - comprehensive developer guide created.

### File List
