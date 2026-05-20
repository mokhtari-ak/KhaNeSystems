---
story_id: "1.1"
story_key: "1-1-bus-devenements-interne-event-bus-type-et-deterministe"
epic_id: "1"
title: "Bus d'Événements interne (Event Bus) typé et déterministe"
status: "done"
last_updated: "2026-05-19"
---

# Story 1.1: Bus d'Événements interne (Event Bus) typé et déterministe

Status: done

## Acceptance Criteria

1. **Given** a message type `IMUData` and a registered subscriber, **When** a module publishes an `IMUData` message to the bus, **Then** all subscribers receive the data via a zero-copy or minimal copy mechanism.
2. **Given** any message exchange on the event bus, **Then** no dynamic memory allocation occurs during the process.
3. **Given** a full message queue, **When** a new high-priority message is published, **Then** the oldest message in the queue is overwritten to accommodate the new one (Ouroboros policy).
4. **Given** a message exchange, **Then** the implementation must use C++20 features (concepts, templates) and be `noexcept`.

## Tasks / Subtasks

- [x] Task 1: Définir les interfaces de base de l'Event Bus (AC: 1, 2, 4)
  - [x] Créer `libs/internal_communication/Inc/event_bus.hpp`
  - [x] Implémenter le concept `event_message` pour valider les types de messages à la compilation.
- [x] Task 2: Implémenter le buffer circulaire déterministe (Ouroboros) (AC: 3)
  - [x] Créer `libs/internal_communication/Inc/ouroboros_buffer.hpp`
  - [x] Implémenter une structure de données `StaticQueue` avec politique d'écrasement.
- [x] Task 3: Implémenter le mécanisme de Publication/Abonnement (AC: 1)
  - [x] Développer la classe `publisher` et `topic`.
  - [x] Assurer le support multi-abonnés via liste intrusive (sans allocation).
- [x] Task 4: Intégrer avec RtosAbstract pour la synchronisation ISR (AC: 4)
  - [x] Utiliser `std::atomic` C++20 pour les index et la gestion de la liste d'abonnés (lock-free registration).
- [x] Task 5: Tests unitaires et validation (AC: 1, 2, 3)
  - [x] Créer un scénario de test avec `IMUData` simulé et multi-abonnés.
  - [x] Vérifier la politique d'écrasement (Ouroboros) via tests unitaires.

## Dev Agent Record
### Implementation Plan
1. Extension de `event_bus.hpp` pour inclure les classes `topic` et `publisher`.
2. Utilisation d'une liste chaînée intrusive pour les abonnés afin de garantir zéro allocation dynamique tout en permettant un nombre flexible d'abonnés par topic.
3. Implémentation de `buffered_subscriber` combinant le noeud d'abonné et le buffer `ouroboros_buffer`.
4. Sécurisation des accès via `std::atomic` et `std::memory_order` pour supporter les échanges entre tâches RTOS et ISR.

### Completion Notes
Le bus d'événements est entièrement fonctionnel et respecte les contraintes de temps réel critique. La communication est typée (via concepts C++20), déterministe et sans aucune allocation de mémoire sur le tas. La politique Ouroboros est appliquée au niveau de chaque abonné, garantissant que les données fraîches ne sont jamais bloquées.

## File List
- libs/internal_communication/Inc/event_bus.hpp
- libs/internal_communication/Inc/ouroboros_buffer.hpp
- libs/internal_communication/Inc/ouroboros_buffer.tpp
- tests/unit/internal_communication/test_ouroboros_buffer.cpp

## Change Log
- [2026-05-19] Implémentation complète du pattern Mediator Statique.
- [2026-05-19] Ajout du support multi-abonnés et des tests Pub/Sub.

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
