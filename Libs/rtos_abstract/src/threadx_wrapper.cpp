#include "rtos_control.hpp"

#ifdef USE_THREADX_BACKEND
#include "tx_api.h"

namespace rtos {

    static_mutex::static_mutex() noexcept {
        static_assert(sizeof(TX_MUTEX) <= 128, "Mutex storage too small for ThreadX");
        TX_MUTEX* mutex_ptr = reinterpret_cast<TX_MUTEX*>(m_storage);
        if (tx_mutex_create(mutex_ptr, const_cast<CHAR*>("Mutex"), TX_NO_INHERIT) == TX_SUCCESS) {
            m_handle = mutex_ptr;
        }
    }

    static_mutex::~static_mutex() {
        if (m_handle) tx_mutex_delete(static_cast<TX_MUTEX*>(m_handle));
    }

    void static_mutex::lock() noexcept {
        if (m_handle) tx_mutex_get(static_cast<TX_MUTEX*>(m_handle), TX_WAIT_FOREVER);
    }

    void static_mutex::unlock() noexcept {
        if (m_handle) tx_mutex_put(static_cast<TX_MUTEX*>(m_handle));
    }

    bool static_mutex::try_lock(uint32_t timeout_ms) noexcept {
        if (!m_handle) return false;
        // Correction overflow : calcul en 64 bits si nécessaire
        UINT ticks = (static_cast<uint64_t>(timeout_ms) * TX_TIMER_TICKS_PER_SECOND) / 1000;
        return tx_mutex_get(static_cast<TX_MUTEX*>(m_handle), ticks) == TX_SUCCESS;
    }

    void kernel::start() noexcept {
        // ThreadX démarre différemment via tx_kernel_enter
        // Note: cette fonction doit être appelée depuis le main.cpp
        tx_kernel_enter();
    }

    uint64_t kernel::get_tick_count() noexcept {
        return static_cast<uint64_t>(tx_time_get());
    }

    void kernel::delay(uint32_t ms) noexcept {
        UINT ticks = (static_cast<uint64_t>(ms) * TX_TIMER_TICKS_PER_SECOND) / 1000;
        tx_thread_sleep(ticks);
    }

} // namespace rtos
#endif
