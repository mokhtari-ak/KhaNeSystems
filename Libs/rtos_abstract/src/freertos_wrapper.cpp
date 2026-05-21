#include "rtos_control.hpp"

#ifdef USE_FREERTOS_BACKEND
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

namespace rtos {

    static_mutex::static_mutex() noexcept {
        static_assert(sizeof(StaticSemaphore_t) <= 128, "Mutex storage too small for FreeRTOS");
        m_handle = xSemaphoreCreateMutexStatic(reinterpret_cast<StaticSemaphore_t*>(m_storage));
    }

    static_mutex::~static_mutex() {
        // En FreeRTOS static, pas besoin de supprimer explicitement le handle s'il est statique
    }

    void static_mutex::lock() noexcept {
        if (m_handle) {
            xSemaphoreTake(static_cast<SemaphoreHandle_t>(m_handle), portMAX_DELAY);
        }
    }

    void static_mutex::unlock() noexcept {
        if (m_handle) {
            xSemaphoreGive(static_cast<SemaphoreHandle_t>(m_handle));
        }
    }

    bool static_mutex::try_lock(uint32_t timeout_ms) noexcept {
        if (!m_handle) return false;
        return xSemaphoreTake(static_cast<SemaphoreHandle_t>(m_handle), pdMS_TO_TICKS(timeout_ms)) == pdTRUE;
    }

    void kernel::start() noexcept {
        vTaskStartScheduler();
    }

    uint64_t kernel::get_tick_count() noexcept {
        return static_cast<uint64_t>(xTaskGetTickCount());
    }

    void kernel::delay(uint32_t ms) noexcept {
        vTaskDelay(pdMS_TO_TICKS(ms));
    }

} // namespace rtos
#endif
