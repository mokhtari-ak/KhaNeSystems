// rtos/freertos/Mutex.hpp
#pragma once
#include "../RtosAbstract.hpp"
#include "FreeRTOS.h"
#include <semphr.h>

namespace Rtos::FreeRTOS {

class Mutex : public Rtos::Abstract::IMutex {
    SemaphoreHandle_t handle_;
public:
    Mutex() noexcept : handle_(xSemaphoreCreateMutexStatic(nullptr)) { }
    ~Mutex() noexcept override { if(handle_) vSemaphoreDelete(handle_); }
    void lock() noexcept override { xSemaphoreTake(handle_, portMAX_DELAY); }
    bool try_lock() noexcept override { return xSemaphoreTake(handle_, 0) == pdTRUE; }
    bool try_lock_for(std::chrono::milliseconds timeout) noexcept override {
        return xSemaphoreTake(handle_, pdMS_TO_TICKS(timeout.count())) == pdTRUE;
    }
    void unlock() noexcept override { xSemaphoreGive(handle_); }
};

} // namespace Rtos::FreeRTOS
