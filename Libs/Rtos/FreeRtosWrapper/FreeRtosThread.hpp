#pragma once

#include "../RtosAbstract/Thread.hpp" // L'interface IThread
#include <functional>
#include <cstdint>
#include <cstring>
#include <utility>
#include <memory>

// Inclure les en-têtes FreeRTOS.
#include "FreeRTOS.h"
#include "task.h"

namespace Rtos::FreeRtosWrapper {

/**
 * @brief Implémentation concrète de l'interface IThread utilisant les Tâches FreeRTOS.
 */
class FreeRtosThread final : public Abstract::IThread {
public:
    /**
     * @brief Construit un objet FreeRtosThread.
     */
    FreeRtosThread() = default;

    /**
     * @brief Destructeur. Supprime la tâche si elle est toujours active (non recommandée).
     * Normalement, une tâche doit se supprimer elle-même après exécution.
     */
    ~FreeRtosThread() override {
        // En cas de destruction de l'objet avant que la tâche ne soit terminée,
        // nous devons nous assurer que nous ne laissons pas une ressource orpheline.
        if (m_taskHandle != nullptr) {
            // NOTE: Supprimer une tâche FreeRTOS de l'extérieur est potentiellement dangereux
            // si elle est dans un état instable ou détient des mutexes.
            vTaskDelete(m_taskHandle);
            m_taskHandle = nullptr;
        }
    }

    // Suppression de la copie et de l'affectation pour garantir l'unicité de la TaskHandle_t.
    FreeRtosThread(const FreeRtosThread&) = delete;
    FreeRtosThread& operator=(const FreeRtosThread&) = delete;

    // --- Implémentations de l'interface IThread ---

    /**
     * @brief Démarre l'exécution de la tâche FreeRTOS.
     * @param entry La fonction C++ (std::function) à exécuter par la tâche.
     * @param name Nom de la tâche (tronqué si trop long).
     * @param stackSizeBytes Taille de la pile en octets.
     * @param priority Priorité de la tâche (doit correspondre aux constantes FreeRTOS).
     * @return true Si la tâche a été créée avec succès.
     * @return false En cas d'échec (mémoire insuffisante, par exemple).
     */
    bool start(std::function<void()> entry, const char* name = nullptr,
               uint32_t stackSizeBytes = 1024, int priority = 1) noexcept override {
        
        // La tâche ne peut être démarrée qu'une seule fois
        if (m_taskHandle != nullptr) {
            return false;
        }

        // Stocker la fonction d'entrée (closure)
        m_entryFunction = std::move(entry);

        // Conversion des paramètres
        const UBaseType_t uxPriority = (UBaseType_t)priority;
        // La taille de la pile dans FreeRTOS est généralement en mots (StackType_t), 
        // pas en octets. Nous devons convertir :
        const configSTACK_DEPTH_TYPE uxStackDepth = (configSTACK_DEPTH_TYPE)(stackSizeBytes / sizeof(StackType_t));
        
        // Si le calcul ci-dessus donne 0 pour une pile trop petite, on prend la taille par défaut
        if (uxStackDepth == 0) {
            return false; // Pile trop petite ou erreur de calcul
        }

        // Détermination du nom (FreeRTOS a une limite de longueur)
        char taskNameBuffer[configMAX_TASK_NAME_LEN];
        if (name != nullptr) {
            strncpy(taskNameBuffer, name, configMAX_TASK_NAME_LEN);
            taskNameBuffer[configMAX_TASK_NAME_LEN - 1] = '\0'; // Assurer la nullité
        } else {
            // Nom par défaut si non spécifié
            strcpy(taskNameBuffer, "RtosThread");
        }

        // Crée la tâche FreeRTOS.
        // On passe 'this' (le pointeur vers l'instance de FreeRtosThread) comme paramètre pvParameters.
        BaseType_t result = xTaskCreate(
            taskEntry,            // Fonction d'entrée statique FreeRTOS
            taskNameBuffer,       // Nom
            uxStackDepth,         // Taille de la pile (en mots)
            this,                 // Paramètre à passer (pointeur vers 'this')
            uxPriority,           // Priorité
            &m_taskHandle         // Handle de la tâche créé
        );

        return (result == pdPASS);
    }

    /**
     * @brief Attendre la fin de l'exécution de la tâche.
     * FreeRTOS ne fournit pas de fonction 'join' standard. Pour simuler cela,
     * nous utiliserions normalement des mécanismes de synchronisation (Sémaphore/Notification).
     * * Dans cette implémentation, nous ne pouvons pas bloquer indéfiniment sans savoir quand la tâche 
     * s'est terminée. Pour respecter l'interface, nous allons laisser cette méthode vide 
     * ou mettre une assertion, car un "join" bloquant n'est pas idiomatique dans FreeRTOS 
     * pour les tâches qui se suppriment elles-mêmes.
     * * Si la tâche est terminée, son handle n'est plus valide. Nous pourrions potentiellement 
     * utiliser un sémaphore interne, mais cela complexifierait trop le wrapper.
     */
    void join() noexcept override {
        // NOTE IMPORTANTE: L'implémentation d'un 'join' fiable dans FreeRTOS est complexe.
        // Si la tâche n'appelle pas 'vTaskDelete(NULL)' à la fin, elle ne se termine jamais.
        // Laissons vide ici en considérant que la tâche se termine par elle-même.
        // Une implémentation réelle nécessiterait un mécanisme de synchronisation interne.
    }

    /**
     * @brief Vérifie si la tâche est en cours d'exécution.
     * Cette vérification est basée uniquement sur la validité du handle après la création.
     * Elle ne garantit pas que la tâche est réellement en cours d'exécution (elle pourrait être bloquée).
     * @return true Si le handle de tâche est valide.
     */
    bool isRunning() const noexcept override {
        // Après l'appel à xTaskCreate, le handle n'est plus nullptr si la création a réussi.
        // Si la tâche se supprime elle-même, elle devrait appeler une méthode pour notifier 
        // l'objet wrapper et réinitialiser m_taskHandle à nullptr.
        // Sans cela, nous ne pouvons vérifier que la création.
        return m_taskHandle != nullptr;
    }

private:
    TaskHandle_t m_taskHandle = nullptr;
    std::function<void()> m_entryFunction;

    /**
     * @brief Point d'entrée statique C-style requis par FreeRTOS.
     * Il convertit le pointeur void* en pointeur d'instance FreeRtosThread et appelle 
     * la fonction d'entrée C++.
     * @param pvParameters Pointeur vers l'instance de FreeRtosThread.
     */
    static void taskEntry(void* pvParameters) {
        // Récupérer le pointeur de l'instance
        FreeRtosThread* instance = static_cast<FreeRtosThread*>(pvParameters);

        if (instance != nullptr) {
            // 1. Exécuter la fonction d'entrée C++ (le cœur de la tâche)
            if (instance->m_entryFunction) {
                instance->m_entryFunction();
            }
            
            // 2. Nettoyage : Une tâche FreeRTOS DOIT se supprimer elle-même si elle se termine.
            // (Sauf si c'est une tâche qui ne se termine jamais, comme une boucle while(1)).
            
            // On peut invalider le handle ici pour indiquer que l'exécution est terminée.
            instance->m_taskHandle = nullptr; 
        }

        // Supprime la tâche actuelle. Le paramètre NULL est pour la tâche actuelle.
        vTaskDelete(NULL);
    }
};

} // namespace Rtos::FreeRtosWrapper