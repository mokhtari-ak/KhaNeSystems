#pragma once
#include <cstdint>
#include <cstddef>

namespace rtos {

    /**
     * @brief Priorités des tâches unifiées pour le firmware.
     */
    enum class priority : uint8_t {
        idle = 0,
        low,
        normal,
        high,
        real_time,
        critical
    };

    /**
     * @brief Interface de base pour toute tâche/thread du système.
     */
    class thread_interface {
    public:
        virtual ~thread_interface() = default;
        virtual void start() noexcept = 0;
        virtual void suspend() noexcept = 0;
        virtual void resume() noexcept = 0;
        virtual const char* get_name() const noexcept = 0;
    };

    /**
     * @brief Interface pour les Mutex (exclusion mutuelle).
     */
    class mutex_interface {
    public:
        virtual ~mutex_interface() = default;
        virtual void lock() noexcept = 0;
        virtual void unlock() noexcept = 0;
        virtual bool try_lock(uint32_t timeout_ms) noexcept = 0;
    };

} // namespace rtos
