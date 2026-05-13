// rtos/freertos/Mutex.hpp
#pragma once
#include "Mutex.hpp" // Inclure l'interface IMutex
#include "FreeRTOS.h"
#include <semphr.h>

namespace Rtos::FreeRtosWrapper {
	template <const char *name = nullptr>
	class FreeRtosMutex : public Rtos::Abstract::IMutex {
    public:
        FreeRtosMutex() noexcept : handle_(xSemaphoreCreateMutexStatic(nullptr)) { }
		
		~FreeRtosMutex() noexcept override { if(handle_) vSemaphoreDelete(handle_); }
		
		void lock() noexcept override { xSemaphoreTake(handle_, portMAX_DELAY); }
		
		bool try_lock() noexcept override { return xSemaphoreTake(handle_, 0) == pdTRUE; }
		
		bool try_lock_for(std::chrono::milliseconds timeout) noexcept override {
			return xSemaphoreTake(handle_, pdMS_TO_TICKS(timeout.count())) == pdTRUE;
		}
		
		void unlock() noexcept override { xSemaphoreGive(handle_); }
  private:
		SemaphoreHandle_t handle_;
};

} // namespace Rtos::FreeRtosWrapper
