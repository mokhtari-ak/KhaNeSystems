#pragma once
#include "rtos_abstract.hpp"
#include <cstdint>

namespace rtos {

    class static_mutex : public mutex_interface {
    public:
        static_mutex() noexcept;
        ~static_mutex() override;

        void lock() noexcept override;
        void unlock() noexcept override;
        bool try_lock(uint32_t timeout_ms) noexcept override;

    private:
        void* m_handle{nullptr};
        // Stockage opaque pour StaticSemaphore_t ou TX_MUTEX
        // Taille conservatrice validée par static_assert dans l'implémentation
        alignas(void*) uint8_t m_storage[128]{}; 
    };

    /**
     * @brief Contrôle global du noyau RTOS.
     */
    class kernel {
    public:
        static void start() noexcept;
        static uint64_t get_tick_count() noexcept;
        static void delay(uint32_t ms) noexcept;
    };

} // namespace rtos
