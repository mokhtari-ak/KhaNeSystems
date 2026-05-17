#include "RtosControl.hpp"

#ifdef USE_FREERTOS_BACKEND
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

namespace Rtos {

    StaticMutex::StaticMutex() noexcept {
        m_handle = xSemaphoreCreateMutexStatic(reinterpret_cast<StaticSemaphore_t*>(m_storage));
    }

    StaticMutex::~StaticMutex() {
        // En FreeRTOS static, pas besoin de supprimer explicitement le handle s'il est statique
    }

    void StaticMutex::lock() noexcept {
        if (m_handle) xSemaphoreTake(static_cast<SemaphoreHandle_t>(m_handle), portMAX_DELAY);
    }

    void StaticMutex::unlock() noexcept {
        if (m_handle) xSemaphoreGive(static_cast<SemaphoreHandle_t>(m_handle));
    }

    bool StaticMutex::tryLock(uint32_t timeoutMs) noexcept {
        if (!m_handle) return false;
        return xSemaphoreTake(static_cast<SemaphoreHandle_t>(m_handle), pdMS_TO_TICKS(timeoutMs)) == pdTRUE;
    }

    void Kernel::start() noexcept {
        vTaskStartScheduler();
    }

    uint64_t Kernel::getTickCount() noexcept {
        return static_cast<uint64_t>(xTaskGetTickCount());
    }

    void Kernel::delay(uint32_t ms) noexcept {
        vTaskDelay(pdMS_TO_TICKS(ms));
    }

} // namespace Rtos
#endif
