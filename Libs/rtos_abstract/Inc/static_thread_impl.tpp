#pragma once

#ifdef USE_FREERTOS_BACKEND
#include "FreeRTOS.h"
#include "task.h"

namespace rtos {
    template<size_t StackSize>
    void static_thread<StackSize>::start() noexcept {
        // Validation de la taille de stockage opaque
        static_assert(sizeof(StaticTask_t) <= 192, "TCB storage too small for FreeRTOS");

        if (m_handle == nullptr) {
            // FreeRTOS utilise des mots (words) pour la pile
            // On s'assure que StackSize est aligné sur StackType_t
            constexpr uint32_t stack_depth = StackSize / sizeof(StackType_t);
            
            m_handle = xTaskCreateStatic(
                m_func,
                m_name,
                stack_depth,
                m_arg,
                static_cast<UBaseType_t>(m_priority),
                reinterpret_cast<StackType_t*>(m_stack.data()),
                reinterpret_cast<StaticTask_t*>(m_tcb_storage)
            );
        }
    }

    template<size_t StackSize>
    void static_thread<StackSize>::suspend() noexcept {
        if (m_handle) vTaskSuspend(static_cast<TaskHandle_t>(m_handle));
    }

    template<size_t StackSize>
    void static_thread<StackSize>::resume() noexcept {
        if (m_handle) vTaskResume(static_cast<TaskHandle_t>(m_handle));
    }
}
#endif

#ifdef USE_THREADX_BACKEND
#include "tx_api.h"

namespace rtos {
    template<size_t StackSize>
    void static_thread<StackSize>::start() noexcept {
        // Validation de la taille de stockage opaque
        static_assert(sizeof(TX_THREAD) <= 192, "TCB storage too small for ThreadX");

        if (m_handle == nullptr) {
            TX_THREAD* thread_ptr = reinterpret_cast<TX_THREAD*>(m_tcb_storage);

            // Adaptateur pour la signature ThreadX (ULONG au lieu de void*)
            auto tx_func = [](ULONG arg) {
                auto* self = reinterpret_cast<static_thread<StackSize>*>(arg);
                self->m_func(self->m_arg);
            };

            UINT status = tx_thread_create(
                thread_ptr,
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
                m_handle = thread_ptr;
            }
        }
    }

    template<size_t StackSize>
    void static_thread<StackSize>::suspend() noexcept {
        if (m_handle) tx_thread_suspend(static_cast<TX_THREAD*>(m_handle));
    }

    template<size_t StackSize>
    void static_thread<StackSize>::resume() noexcept {
        if (m_handle) tx_thread_resume(static_cast<TX_THREAD*>(m_handle));
    }
}
#endif
