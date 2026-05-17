#pragma once
#include <cstdint>
#include <cstddef>

namespace Rtos {

    /**
     * @brief Priorités des tâches unifiées pour le firmware.
     */
    enum class Priority : uint8_t {
        Idle = 0,
        Low,
        Normal,
        High,
        RealTime,
        Critical
    };

    /**
     * @brief Interface de base pour toute tâche/thread du système.
     */
    class ThreadInterface {
    public:
        virtual ~ThreadInterface() = default;
        virtual void start() noexcept = 0;
        virtual void suspend() noexcept = 0;
        virtual void resume() noexcept = 0;
        virtual const char* getName() const noexcept = 0;
    };

    /**
     * @brief Interface pour les Mutex (exclusion mutuelle).
     */
    class MutexInterface {
    public:
        virtual ~MutexInterface() = default;
        virtual void lock() noexcept = 0;
        virtual void unlock() noexcept = 0;
        virtual bool tryLock(uint32_t timeoutMs) noexcept = 0;
    };

} // namespace Rtos
