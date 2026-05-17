#pragma once
#include "RtosAbstract.hpp"
#include <cstdint>

namespace Rtos {

    class StaticMutex : public MutexInterface {
    public:
        StaticMutex() noexcept;
        ~StaticMutex() override;

        void lock() noexcept override;
        void unlock() noexcept override;
        bool tryLock(uint32_t timeoutMs) noexcept override;

    private:
        void* m_handle{nullptr};
        alignas(void*) uint8_t m_storage[128]{}; // Stockage opaque pour StaticSemaphore_t ou TX_MUTEX
    };

    /**
     * @brief Contrôle global du noyau RTOS.
     */
    class Kernel {
    public:
        static void start() noexcept;
        static uint64_t getTickCount() noexcept;
        static void delay(uint32_t ms) noexcept;
    };

} // namespace Rtos
