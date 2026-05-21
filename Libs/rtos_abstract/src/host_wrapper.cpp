#include "rtos_control.hpp"

#ifndef USE_FREERTOS_BACKEND
#ifndef USE_THREADX_BACKEND
// Host simulation implementation
#include <mutex>
#include <chrono>
#include <thread>
#include <new>

namespace rtos {

    struct host_mutex_data {
        std::mutex mtx;
    };

    static_mutex::static_mutex() noexcept {
        static_assert(sizeof(host_mutex_data) <= 128, "Mutex storage too small for Host");
        m_handle = new (m_storage) host_mutex_data();
    }

    static_mutex::~static_mutex() {
        reinterpret_cast<host_mutex_data*>(m_handle)->~host_mutex_data();
    }

    void static_mutex::lock() noexcept {
        if (m_handle) {
            reinterpret_cast<host_mutex_data*>(m_handle)->mtx.lock();
        }
    }

    void static_mutex::unlock() noexcept {
        if (m_handle) {
            reinterpret_cast<host_mutex_data*>(m_handle)->mtx.unlock();
        }
    }

    bool static_mutex::try_lock(uint32_t timeout_ms) noexcept {
        if (!m_handle) return false;
        
        auto* data = reinterpret_cast<host_mutex_data*>(m_handle);
        if (timeout_ms == 0) {
            return data->mtx.try_lock();
        } else {
            // Simplistic timed lock for host simulation
            uint32_t elapsed = 0;
            while (elapsed < timeout_ms) {
                if (data->mtx.try_lock()) return true;
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                elapsed++;
            }
            return false;
        }
    }

    void kernel::start() noexcept {
        // No-op on host
    }

    uint64_t kernel::get_tick_count() noexcept {
        auto now = std::chrono::steady_clock::now();
        // Return milliseconds to better simulate typical RTOS ticks
        return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    }

    void kernel::delay(uint32_t ms) noexcept {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }

} // namespace rtos

#endif
#endif
