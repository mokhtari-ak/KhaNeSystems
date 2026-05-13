#pragma once
#include "Semaphore.hpp"
#include "tx_api.h"
#include <cstdint>

namespace Rtos::ThreadXWrapper
{
	template <uint16_t initial_count = 0, uint16_t max_count = 1, const char *name = nullptr>
	class ThreadXSemaphore : public Rtos::Abstract::ISemaphore
	{
	  public:
		ThreadXSemaphore()
		{
			tx_semaphore_create(&semaphore_, (CHAR *)(name ? name : "ThreadXSem"), initial_count);
		}

		~ThreadXSemaphore() override
		{
			tx_semaphore_delete(&semaphore_);
		}

		void give() noexcept override
		{
			tx_semaphore_put(&semaphore_);
		}

		bool take(uint32_t timeout_ms) noexcept override
		{
			ULONG ticks = (timeout_ms == 0xFFFFFFFF) ? TX_WAIT_FOREVER : (timeout_ms * TX_TIMER_TICKS_PER_SECOND) / 1000;
			return tx_semaphore_get(&semaphore_, ticks) == TX_SUCCESS;
		}

		bool take_from_isr() noexcept override
		{
			// ThreadX n'a pas d'API ISR spécifique pour les sémaphores, on utilise tx_semaphore_get avec 0 timeout
			return tx_semaphore_get(&semaphore_, 0) == TX_SUCCESS;
		}

	  private:
		TX_SEMAPHORE semaphore_;
	};

} // namespace Rtos::ThreadXWrapper