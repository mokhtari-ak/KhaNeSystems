---
stepsCompleted: [1, 2]
inputDocuments: []
session_topic: 'Architecture firmware drone critique (STM32F407, C++20, RTOS agnostique)'
session_goals: 'Définir l''arborescence, le système de build CMake déterministe, et les patterns de drivers/mocks sécurisés.'
selected_approach: 'ai-recommended'
techniques_used: ['Assumption Reversal', 'Morphological Analysis', 'Chaos Engineering']
ideas_generated: []
context_file: ''
---

# Session de Brainstorming : Firmware Drone Critique

## Session Overview

**Topic:** Architecture firmware drone critique (STM32F407, C++20, RTOS agnostique)
**Goals:** Définir l'arborescence, le système de build CMake déterministe, et les patterns de drivers/mocks sécurisés.

### Session Setup

L'objectif est de concevoir un système modulaire respectant des contraintes strictes : C++20 sans exceptions ni RTTI, allocation dynamique interdite en régime permanent, et une abstraction totale vis-à-vis du RTOS (FreeRTOS/ThreadX). La session vise à consolider les choix structurels avant l'implémentation.

## Technique Selection

**Approach:** AI-Recommended Techniques
**Analysis Context:** Architecture firmware drone critique (STM32F407, C++20, RTOS agnostique) with focus on Définir l'arborescence, le système de build CMake déterministe, et les patterns de drivers/mocks sécurisés.

**Recommended Techniques:**

- **Assumption Reversal:** Essentiel pour identifier les points de rupture en inversant les hypothèses de fiabilité du matériel et des couches logicielles.
- **Morphological Analysis:** Permet de mapper systématiquement les combinaisons de transport, drivers et RTOS pour garantir l'agnosticisme.
- **Chaos Engineering:** Stress-test conceptuel pour définir les comportements de la machine d'état en cas de défaillance critique.

**AI Rationale:** Cette séquence passe de l'analyse des risques (Phase 1) à la structuration modulaire (Phase 2) pour finir par la validation des scénarios de sécurité (Phase 3), assurant une architecture robuste par construction.
