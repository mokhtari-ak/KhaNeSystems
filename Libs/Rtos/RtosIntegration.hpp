#ifndef RTOS_INTEGRATION_HPP
#define RTOS_INTEGRATION_HPP

#include "RtosAbstract.hpp"
#include "RtosConfig.hpp"
// --- Définition des macros de sélection ---

// Définissez uniquement l'une de ces macros dans les options du compilateur (ex: -DUSE_FREERTOS)
// #define USE_FREERTOS 
// #define USE_THREADX 

// --- Sélection du Mutex RTOS ---

#if defined(USE_FREERTOS)

#include "FreeRtosMutex.hpp"
    namespace Rtos = FreeRtos; // Alias pour utiliser le namespace FreeRtos
#pragma message "Compilant avec FreeRTOS Mutex Wrapper"

#elif defined(USE_THREADX)

#include "ThreadXMutex.hpp"
    namespace Rtos = ThreadX; // Alias pour utiliser le namespace ThreadX
#pragma message "Compilant avec ThreadX Mutex Wrapper"

#else
    // Si aucune macro n'est définie, générez une erreur de compilation
#error "Aucun RTOS défini! Veuillez définir USE_FREERTOS ou USE_THREADX."

#endif

// Définition de types "génériques" pointant vers l'implémentation choisie
using RtosMutex = Rtos::Mutex;
using RtosLockGuard = RtosAbstract::LockGuard; // LockGuard utilise toujours l'interface commune

#endif // RTOS_INTEGRATION_HPP