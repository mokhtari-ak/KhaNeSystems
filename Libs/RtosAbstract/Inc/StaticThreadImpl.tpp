#pragma once

#ifdef USE_FREERTOS_BACKEND
#include "FreeRTOS.h"
#include "task.h"

namespace Rtos {
    template<size_t StackSize>
    void StaticThread<StackSize>::start() noexcept {
        if (m_handle == nullptr) {
            // FreeRTOS utilise des mots (words) pour la pile, on convertit
            uint32_t stackDepth = StackSize / sizeof(StackType_t);
            
            m_handle = xTaskCreateStatic(
                m_func,
                m_name,
                stackDepth,
                m_arg,
                static_cast<UBaseType_t>(m_priority), // Mapper les priorités si nécessaire
                reinterpret_cast<StackType_t*>(m_stack.data()),
                reinterpret_cast<StaticTask_t*>(m_tcbStorage)
            );
        }
    }

    template<size_t StackSize>
    void StaticThread<StackSize>::suspend() noexcept {
        if (m_handle) vTaskSuspend(static_cast<TaskHandle_t>(m_handle));
    }

    template<size_t StackSize>
    void StaticThread<StackSize>::resume() noexcept {
        if (m_handle) vTaskResume(static_cast<TaskHandle_t>(m_handle));
    }
}
#endif

#ifdef USE_THREADX_BACKEND
#include "tx_api.h"

namespace Rtos {
    template<size_t StackSize>
    void StaticThread<StackSize>::start() noexcept {
        if (m_handle == nullptr) {
            TX_THREAD* threadPtr = reinterpret_cast<TX_THREAD*>(m_tcbStorage);
            
            // Adaptateur pour la signature ThreadX (ULONG au lieu de void*)
            auto tx_func = [](ULONG arg) {
                auto* self = reinterpret_cast<StaticThread<StackSize>*>(arg);
                self->m_func(self->m_arg);
            };

            UINT status = tx_thread_create(
                threadPtr,
                const_cast<CHAR*>(m_name),
                tx_func,
                reinterpret_cast<ULONG>(this),
                m_stack.data(),
                StackSize,
                static_cast<UINT>(m_priority),
                static_cast<UINT>(m_priority),
                TX_NO_TIME_SLICE,
                TX_AUTO_START
            );

            if (status == TX_SUCCESS) {
                m_handle = threadPtr;
            }
        }
    }

    template<size_t StackSize>
    void StaticThread<StackSize>::suspend() noexcept {
        if (m_handle) tx_thread_suspend(static_cast<TX_THREAD*>(m_handle));
    }

    template<size_t StackSize>
    void StaticThread<StackSize>::resume() noexcept {
        if (m_handle) tx_thread_resume(static_cast<TX_THREAD*>(m_handle));
    }
}
#endif
