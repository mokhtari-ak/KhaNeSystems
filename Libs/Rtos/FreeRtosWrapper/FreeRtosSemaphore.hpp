#pragma once

#include "Semaphore.hpp" // L'interface ISemaphore
#include <chrono>

// Inclure les en-têtes FreeRTOS.
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

namespace Rtos::FreeRtosWrapper {

/**
 * @brief Implémentation concrète de l'interface ISemaphore utilisant un sémaphore binaire FreeRTOS.
 */
class FreeRtosSemaphore final : public Rtos::Abstract::ISemaphore {
public:
    /**
     * @brief Construit un nouvel objet FreeRtosSemaphore (binaire).
     * @param initialValue Si true, le sémaphore est initialement disponible (donné).
     */
    explicit FreeRtosSemaphore(bool initialValue = false) {
        // Crée un sémaphore binaire. Il est initialement "pris" (empty).
        m_semaphoreHandle = xSemaphoreCreateBinary();

        // Si l'utilisateur souhaite qu'il soit initialement disponible, on le donne immédiatement.
        if (initialValue && m_semaphoreHandle != nullptr) {
            xSemaphoreGive(m_semaphoreHandle);
        }
        
        // NOTE : Il est crucial de vérifier si la création a échoué (m_semaphoreHandle == nullptr)
        // en cas de manque de mémoire pour le tas FreeRTOS.
    }

    /**
     * @brief Détruit l'objet FreeRtosSemaphore et supprime la ressource FreeRTOS sous-jacente.
     */
    ~FreeRtosSemaphore() override {
        if (m_semaphoreHandle != nullptr) {
            // Supprime le sémaphore FreeRTOS.
            vSemaphoreDelete(m_semaphoreHandle);
            m_semaphoreHandle = nullptr;
        }
    }

    // Suppression de la copie et de l'affectation.
    FreeRtosSemaphore(const FreeRtosSemaphore&) = delete;
    FreeRtosSemaphore& operator=(const FreeRtosSemaphore&) = delete;

    // --- Implémentations de l'interface ISemaphore ---

    /**
     * @brief Libère le sémaphore, le rendant disponible pour une autre tâche.
     * Utilise xSemaphoreGive.
     */
    void give() noexcept override {
        if (m_semaphoreHandle != nullptr) {
            // xSemaphoreGive permet de rendre le sémaphore disponible.
            xSemaphoreGive(m_semaphoreHandle);
        }
    }

    /**
     * @brief Tente de prendre (acquérir) le sémaphore, bloquant pendant le délai spécifié.
     * Utilise xSemaphoreTake.
     * @param timeout Délai d'attente maximal en millisecondes.
     * @return true Si le sémaphore a été pris avec succès avant l'expiration du délai.
     * @return false Si le délai a expiré.
     */
    bool take(std::chrono::milliseconds timeout) noexcept override {
        if (m_semaphoreHandle == nullptr) {
            return false;
        }
        
        // Conversion du délai d'attente en millisecondes vers des Ticks FreeRTOS.
        TickType_t ticksToWait = msToTicks(timeout);

        // xSemaphoreTake est la primitive FreeRTOS.
        BaseType_t result = xSemaphoreTake(m_semaphoreHandle, ticksToWait);

        return (result == pdPASS);
    }

    /**
     * @brief Tente de prendre (acquérir) le sémaphore depuis une routine de service d'interruption (ISR).
     * Utilise xSemaphoreTakeFromISR.
     * @return true Si le sémaphore a été pris avec succès.
     * @return false Si le sémaphore n'était pas disponible immédiatement.
     */
    bool take_from_isr() noexcept override {
        if (m_semaphoreHandle == nullptr) {
            return false;
        }
        
        // Déclaration obligatoire pour la fonction FromISR.
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        
        // xSemaphoreTakeFromISR doit être appelé. Le timeout est toujours 0 (pas d'attente en ISR).
        BaseType_t result = xSemaphoreTakeFromISR(
            m_semaphoreHandle,
            &xHigherPriorityTaskWoken
        );
        
        // NOTE IMPORTANTE :
        // Pour une utilisation complète en ISR, la tâche de plus haute priorité réveillée 
        // DOIT être traitée par un appel à portYIELD_FROM_ISR(xHigherPriorityTaskWoken) 
        // juste après l'appel à cette méthode. 
        // Cependant, cette décision est laissée à l'utilisateur de l'API.

        return (result == pdPASS);
    }

private:
    SemaphoreHandle_t m_semaphoreHandle = nullptr;

    /**
     * @brief Convertit un délai d'attente en std::chrono::milliseconds en Ticks FreeRTOS.
     * @param timeout Délai d'attente en millisecondes.
     * @return TickType_t Délai d'attente en Ticks.
     */
    TickType_t msToTicks(std::chrono::milliseconds timeout) const noexcept {
        uint32_t timeoutMs = timeout.count();
        
        if (timeoutMs == 0) {
            return 0; // Ne pas attendre du tout
        }
        
        // Détection de l'attente infinie (si l'utilisateur utilise une très grande valeur)
        // Nous allons utiliser une valeur conventionnelle (par exemple, la plus grande possible pour un uint32_t)
        // pour représenter l'attente infinie.
        if (timeoutMs == std::chrono::milliseconds::max().count()) { 
             return portMAX_DELAY;
        }

        // Utilise la macro standard de FreeRTOS pour la conversion.
        TickType_t ticks = pdMS_TO_TICKS(timeoutMs);

        // Assurer que le temps d'attente minimum est d'au moins 1 tick si timeoutMs > 0
        if (ticks == 0 && timeoutMs > 0) {
            ticks = 1;
        }

        return ticks;
    }
};

} // namespace Rtos::FreeRtosWrapper