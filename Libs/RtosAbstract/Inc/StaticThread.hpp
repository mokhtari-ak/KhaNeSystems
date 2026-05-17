#pragma once
#include "RtosAbstract.hpp"
#include <array>

namespace Rtos {

    // Forward declarations des types opaques pour éviter d'inclure les headers RTOS
    namespace Backend {
        struct TaskControlBlock; 
    }

    /**
     * @brief Implémentation générique d'une tâche avec allocation statique.
     * @tparam StackSize Taille de la pile en octets.
     */
    template<size_t StackSize>
    class StaticThread : public ThreadInterface {
    public:
        using TaskFunction = void (*)(void*);

        StaticThread(const char* name, Priority priority, TaskFunction func, void* arg = nullptr) noexcept
            : m_name(name), m_priority(priority), m_func(func), m_arg(arg) {}

        void start() noexcept override;
        void suspend() noexcept override;
        void resume() noexcept override;
        const char* getName() const noexcept override { return m_name; }

    private:
        const char* m_name;
        Priority m_priority;
        TaskFunction m_func;
        void* m_arg;

        // Stockage de la pile (aligné pour le matériel)
        alignas(8) std::array<uint8_t, StackSize> m_stack{};

        // Handle opaque vers l'objet RTOS natif
        void* m_handle{nullptr};

        // Stockage opaque pour le bloc de contrôle (TCB)
        // Taille conservatrice pour couvrir FreeRTOS StaticTask_t et ThreadX TX_THREAD
        alignas(void*) uint8_t m_tcbStorage[192]{}; 
    };

} // namespace Rtos

#include "StaticThreadImpl.tpp"
