#pragma once
#include "rtos_abstract.hpp"
#include <array>

namespace rtos {

    /**
     * @brief Implémentation générique d'une tâche avec allocation statique.
     * @tparam StackSize Taille de la pile en octets.
     */
    template<size_t StackSize>
    class static_thread : public thread_interface {
    public:
        using task_function = void (*)(void*);

        static_thread(const char* name, priority prio, task_function func, void* arg = nullptr) noexcept
            : m_name(name), m_priority(prio), m_func(func), m_arg(arg) {}

        void start() noexcept override;
        void suspend() noexcept override;
        void resume() noexcept override;
        const char* get_name() const noexcept override { return m_name; }

    private:
        const char* m_name;
        priority m_priority;
        task_function m_func;
        void* m_arg;

        // Stockage de la pile (aligné pour le matériel)
        alignas(8) std::array<uint8_t, StackSize> m_stack{};

        // Handle opaque vers l'objet RTOS natif
        void* m_handle{nullptr};

        // Stockage opaque pour le bloc de contrôle (TCB)
        // Taille conservatrice pour couvrir FreeRTOS StaticTask_t et ThreadX TX_THREAD
        alignas(void*) uint8_t m_tcb_storage[192]{}; 
    };

} // namespace rtos

#include "static_thread_impl.tpp"
