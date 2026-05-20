---
story_id: "1.3"
story_key: "1-3-gestion-des-priorites-et-ecrasement-rate-limiting-overwrite"
epic_id: "1"
title: "Gestion des priorités et écrasement (Rate-Limiting & Overwrite)"
status: "done"
last_updated: "2026-05-19"
---

# Story 1.3: Gestion des priorites et écrasement (Rate-Limiting & Overwrite)

## User Story
**As a** safety supervisor,
**I want** the communication bus to implement rate-limiting and priority-based overwriting,
**So that** critical data always has space in the queues even during module flooding and diagnostic information is clear.

## Acceptance Criteria

**Given** a full message queue
**When** a new message is published
**Then** the oldest message is overwritten ONLY if its priority is equal or lower than the new message (Priority-Aware Ouroboros).

**Given** a publisher
**When** it exceeds its statically configured frequency (Rate-Limiting)
**Then** the excess messages are discarded side-publisher and a `rate_limit_dropped_count` is incremented.

**Given** a buffer overflow (overwrite)
**Then** an `overwrite_dropped_count` is incremented separately from rate-limiting drops.

**Given** any bus operation, **Then** no dynamic memory allocation occurs, all operations are `noexcept`, and timing variables use `std::atomic` for thread-safety.

## Developer Context

### Foundation Analysis
Cette story transforme le bus d'événements en un système robuste capable de résister aux "tempêtes" de messages. La distinction entre les pertes à l'émission (Rate-Limit) et à la réception (Overwrite) est cruciale pour le Supervisor (Epic 2).

### Technical Guardrails
- **Language**: C++20 (Embedded Subset).
- **Priority**: Définir un `enum class message_priority : uint8_t { low, normal, high, critical }` dans `event_bus.hpp`.
- **Timing**: Utiliser `std::atomic<uint64_t>` pour `last_publish_time` dans `publisher`.
- **Determinism**: L'algorithme de recherche de la donnée à écraser doit rester simple pour garantir le temps réel (idéalement vérifier juste la tête du buffer).

### Architecture Compliance
- **Location**: `libs/internal_communication/Inc/`
- **Rate-Limiting Statique**: Préférer une configuration via un trait de Topic ou un paramètre de template `MinIntervalUs` sur la classe `publisher`.
- **Diagnostic**: Chaque `buffered_subscriber` doit exposer ses compteurs de perte.

### Library & Framework Requirements
- Modifier `libs/internal_communication/Inc/event_bus.hpp` et `ouroboros_buffer.hpp`.
- Utiliser `libs/rtos_abstract/Inc/rtos_control.hpp`.

### Testing Requirements
- **Location**: `tests/unit/internal_communication/test_robustness.cpp`.
- **Priority Inversion Test**: Vérifier qu'un message `low` ne peut pas écraser un message `critical` dans un buffer plein.
- **Diagnostic Test**: Publier à 2x la fréquence autorisée et vérifier que `rate_limit_dropped_count` capture exactement 50% des pertes.

## Tasks/Subtasks
- [x] Task 1: Gestion des Priorités dans le Bus
  - [x] Ajouter `message_priority` au concept `event_message` (chaque message doit porter sa priorité).
  - [x] Modifier `ouroboros_buffer::push()` pour implémenter la règle d'écrasement sélectif.
- [x] Task 2: Implémentation du Rate-Limiting Thread-Safe
  - [x] Ajouter `std::atomic<uint64_t> last_publish_time` à `publisher`.
  - [x] Ajouter un compteur atomique `rate_limit_dropped_count`.
  - [x] Intégrer la vérification temporelle dans `publisher::publish()`.
- [x] Task 3: API de Diagnostic Étendue
  - [x] Ajouter `get_overwrite_drops()` et `get_rate_limit_drops()` aux interfaces concernées.
- [x] Task 4: Tests Unitaires de Robustesse
  - [x] Valider la protection des messages critiques et la précision des compteurs de perte.

### Review Findings
- [x] [Review][Patch] Race Condition dans le Buffer Lock-Free [libs/internal_communication/Inc/ouroboros_buffer.tpp]
- [x] [Review][Patch] Incohérence des Unités Temporelles (ms vs us) [libs/internal_communication/Inc/event_bus.hpp]
- [x] [Review][Patch] Logique de Wrap-around du Timer défectueuse [libs/internal_communication/Inc/event_bus.hpp:115]
- [x] [Review][Patch] Limitation de l'Abonné Intrusif (multi-topic support) [libs/internal_communication/Inc/event_bus.hpp]
- [x] [Review][Patch] Chemins CMake fragiles [Tests/CMakeLists.txt]
- [x] [Review][Patch] Tests incomplets (appel manquant dans main) [tests/unit/internal_communication/test_robustness.cpp]

## Previous Story Intelligence
- `ouroboros_buffer` possède déjà un `m_dropped_count` générique, il doit être renommé et spécialisé.
- L'horloge système `rtos::kernel::get_tick_count()` a été confirmée comme retournant des microsecondes dans la story 1.2.

---
**Status:** review
**Completion Note:** Implémentation de la robustesse du bus terminée. Le système supporte désormais la priorité d'écrasement et le rate-limiting configurable par éditeur. Les diagnostics de perte sont séparés pour faciliter l'analyse post-incident.

## Dev Agent Record
### Implementation Plan
1. **Priorités** : Introduction de `message_priority` dans `event_bus.hpp` et mise à jour du concept `event_message`.
2. **Ouroboros** : Mise à jour de `ouroboros_buffer.tpp` pour protéger les messages de haute priorité.
3. **Rate-Limiting** : Ajout d'un paramètre template `MinIntervalUs` à la classe `publisher` pour un contrôle fin du débit.
4. **Diagnostics** : Ajout de compteurs atomiques pour les pertes par écrasement et par limitation de débit.
5. **Validation** : Création de tests de robustesse simulant des conditions de surcharge.

### File List
- `libs/internal_communication/Inc/event_bus.hpp` (UPDATE)
- `libs/internal_communication/Inc/ouroboros_buffer.hpp` (UPDATE)
- `libs/internal_communication/Inc/ouroboros_buffer.tpp` (UPDATE)
- `libs/common/sensor_types.hpp` (UPDATE)
- `libs/common/Inc/logger_types.hpp` (UPDATE)
- `tests/unit/internal_communication/test_ouroboros_buffer.cpp` (UPDATE)
- `tests/unit/internal_communication/test_robustness.cpp` (NEW)
- `tests/CMakeLists.txt` (UPDATE)

### Change Log
- [2026-05-19] Implémentation complète de la gestion des priorités et du rate-limiting.
- [2026-05-19] Passage du statut à 'review'.
