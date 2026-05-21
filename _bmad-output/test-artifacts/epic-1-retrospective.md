# Retrospective: Epic 1 - Socle de Communication & Diagnostic

## Introduction
Cette rétrospective couvre l'implémentation de l'Epic 1, focalisée sur les fondations critiques du système KhaNeSystems : Event Bus, Logging, Gestion des Priorités et Stockage.

## Ce qui a bien fonctionné
- **Approche "Zero Dynamic Allocation"** : Le choix strict de bannir l'allocation dynamique dès le départ a facilité la stabilité et le déterminisme du système.
- **Concepts C++20** : L'utilisation intensive des concepts a permis une détection précoce des erreurs de typage, renforçant la robustesse de l'Event Bus.
- **Politique Ouroboros** : Le pattern de buffer circulaire avec écrasement automatique s'est avéré efficace pour garantir que le système reste réactif en toutes circonstances, même sous forte charge.
- **Séparation des préoccupations** : La création de couches d'abstraction (`libs/rtos_abstract`, `libs/internal_communication`) a permis de découpler la logique métier du matériel STM32, facilitant les tests unitaires sur machine hôte.

## Domaines d'amélioration (Leçons apprises)
- **Complexité des tests unitaires** : La difficulté de compiler le code C++20 sur l'hôte (nécessité de wrappers complexes) a ralenti la boucle de rétroaction. Il est nécessaire de simplifier la couche d'abstraction pour les tests à l'avenir.
- **Gestion de la concurrence** : Des vulnérabilités ont été identifiées dans la gestion des Mutex pour les accès ISR (Inversion de priorité). La stratégie de synchronisation devra être revue pour utiliser des primitives plus légères comme des `lock-free buffers` là où c'est possible.
- **Macros et sécurité** : Certaines macros de logging se sont révélées trop rigides et sujettes aux erreurs (mismatch d'arguments). Une approche plus typée (type-safe variadic templates) est recommandée.
- **Alignement et empaquetage** : L'usage des attributs `packed` a révélé des risques d'accès mémoire non aligné, nécessitant une analyse rigoureuse lors de la définition des structures de données (PODs).

## Recommandations pour les prochaines épiques
1. **Prioriser les primitives Lock-Free** : Pour les épiques suivantes, éviter systématiquement les Mutex au profit de structures de données Lock-Free pour minimiser les interruptions de priorité.
2. **Standardiser les wrappers d'abstraction** : Créer un jeu d'outils de test plus robuste pour les fonctionnalités système afin de réduire le temps passé sur la gestion des wrappers hôtes.
3. **Améliorer la sécurité des macros** : Remplacer les macros de logging par des fonctions template inline type-safe pour capturer les erreurs de typage au moment de la compilation.
4. **Renforcer les tests d'intégration matérielle** : Bien que les tests unitaires soient complets, prévoir un jalon d'intégration HITL (Hardware-in-the-loop) plus tôt dans le cycle pour valider les comportements liés à la latence matérielle réelle.
