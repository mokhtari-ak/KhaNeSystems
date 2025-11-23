#pragma once

#include "../RtosAbstract/Queue.hpp" // L'interface IQueue doit être accessible
#include <cstdint>
#include <cstddef>

// Inclure les en-têtes FreeRTOS.
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

namespace Rtos::FreeRtosWrapper {

/**
 * @brief Implémentation concrète de l'interface IQueue utilisant les files d'attente FreeRTOS.
 */
class FreeRtosQueue final : public Abstract::IQueue {
public:
    /**
     * @brief Construit un nouvel objet FreeRtosQueue.
     * @param maxQueueLength La taille maximale de la file d'attente (nombre d'éléments).
     * @param itemSize La taille de chaque élément de la file d'attente en octets.
     */
    FreeRtosQueue(UBaseType_t maxQueueLength, UBaseType_t itemSize)
        : m_itemSize(itemSize) {
        // Crée la file d'attente FreeRTOS.
        // xQueueCreate retourne NULL en cas d'échec (souvent par manque de mémoire).
        m_queueHandle = xQueueCreate(maxQueueLength, itemSize);
        
        // NOTE : Il est conseillé d'ajouter une gestion d'erreur ici 
        // (par exemple, lancer une exception ou un arrêt système).
    }

    /**
     * @brief Détruit l'objet FreeRtosQueue et supprime la file d'attente FreeRTOS sous-jacente.
     */
    ~FreeRtosQueue() override {
        if (m_queueHandle != nullptr) {
            // Supprime la file d'attente FreeRTOS.
            vQueueDelete(m_queueHandle);
            m_queueHandle = nullptr;
        }
    }

    // Suppression de la copie et de l'affectation pour garantir l'unicité de la QueueHandle_t.
    FreeRtosQueue(const FreeRtosQueue&) = delete;
    FreeRtosQueue& operator=(const FreeRtosQueue&) = delete;

    // --- Implémentations de l'interface IQueue ---

    /**
     * @brief Envoie un élément à la file d'attente.
     * @param item Pointeur vers l'élément à envoyer.
     * @param size Taille de l'élément (doit correspondre à itemSize).
     * @param timeoutMs Délai d'attente maximal en millisecondes.
     * @return true Si l'envoi a réussi.
     * @return false Si la file d'attente est pleine et que le délai d'attente a expiré, ou en cas d'erreur de taille.
     */
    bool send(const void* item, size_t size, uint32_t timeoutMs) noexcept override {
        // Vérification de sécurité pour s'assurer que la queue est valide et la taille correcte.
        if (m_queueHandle == nullptr || size != m_itemSize) {
            return false;
        }

        // Conversion du délai d'attente en millisecondes vers des Ticks FreeRTOS.
        TickType_t ticksToWait = msToTicks(timeoutMs);

        // xQueueSend est la primitive FreeRTOS.
        BaseType_t result = xQueueSend(m_queueHandle, item, ticksToWait);

        return (result == pdPASS);
    }

    /**
     * @brief Reçoit un élément de la file d'attente.
     * @param outItem Pointeur où l'élément reçu sera copié.
     * @param size Taille de l'élément (doit correspondre à itemSize).
     * @param timeoutMs Délai d'attente maximal en millisecondes.
     * @return true Si la réception a réussi.
     * @return false Si la file d'attente est vide et que le délai d'attente a expiré, ou en cas d'erreur.
     */
    bool receive(void* outItem, size_t size, uint32_t timeoutMs) noexcept override {
        // Vérification de sécurité.
        if (m_queueHandle == nullptr || outItem == nullptr || size != m_itemSize) {
            return false;
        }

        // Conversion du délai d'attente en millisecondes vers des Ticks FreeRTOS.
        TickType_t ticksToWait = msToTicks(timeoutMs);

        // xQueueReceive est la primitive FreeRTOS.
        BaseType_t result = xQueueReceive(m_queueHandle, outItem, ticksToWait);

        return (result == pdPASS);
    }

private:
    QueueHandle_t m_queueHandle = nullptr;
    UBaseType_t m_itemSize = 0;

    /**
     * @brief Convertit un délai d'attente en millisecondes en Ticks FreeRTOS.
     * @param timeoutMs Délai d'attente en millisecondes.
     * @return TickType_t Délai d'attente en Ticks.
     */
    TickType_t msToTicks(uint32_t timeoutMs) const noexcept {
        if (timeoutMs == 0) {
            return 0; // Ne pas attendre du tout
        }
        if (timeoutMs == UINT32_MAX) {
            return portMAX_DELAY; // Attendre indéfiniment
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