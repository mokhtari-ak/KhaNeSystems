#include "RtosControl.hpp"

#ifdef USE_THREADX_BACKEND
#include "tx_api.h"

namespace Rtos {

    StaticMutex::StaticMutex() noexcept {
        TX_MUTEX* mutexPtr = reinterpret_cast<TX_MUTEX*>(m_storage);
        if (tx_mutex_create(mutexPtr, const_cast<CHAR*>("Mutex"), TX_NO_INHERIT) == TX_SUCCESS) {
            m_handle = mutexPtr;
        }
    }

    StaticMutex::~StaticMutex() {
        if (m_handle) tx_mutex_delete(static_cast<TX_MUTEX*>(m_handle));
    }

    void StaticMutex::lock() noexcept {
        if (m_handle) tx_mutex_get(static_cast<TX_MUTEX*>(m_handle), TX_WAIT_FOREVER);
    }

    void StaticMutex::unlock() noexcept {
        if (m_handle) tx_mutex_put(static_cast<TX_MUTEX*>(m_handle));
    }

    bool StaticMutex::tryLock(uint32_t timeoutMs) noexcept {
        if (!m_handle) return false;
        UINT ticks = (timeoutMs * TX_TIMER_TICKS_PER_SECOND) / 1000;
        return tx_mutex_get(static_cast<TX_MUTEX*>(m_handle), ticks) == TX_SUCCESS;
    }

    void Kernel::start() noexcept {
        // ThreadX démarre différemment via tx_kernel_enter
        // Note: cette fonction doit être appelée depuis le main.c
        tx_kernel_enter();
    }

    uint64_t Kernel::getTickCount() noexcept {
        return static_cast<uint64_t>(tx_time_get());
    }

    void Kernel::delay(uint32_t ms) noexcept {
        UINT ticks = (ms * TX_TIMER_TICKS_PER_SECOND) / 1000;
        tx_thread_sleep(ticks);
    }

} // namespace Rtos
#endif
