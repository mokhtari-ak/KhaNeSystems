#pragma once
#include "Mutex.hpp"
#include <chrono>
#include <tx_api.h>

namespace Rtos::ThreadXWrapper
{
	template <const char *name = nullptr>
	class ThreadXMutex : public Rtos::Abstract::IMutex
	{
	  public:
		ThreadXMutex()
		{
			tx_mutex_create(&m_mutex, const_cast<CHAR *>(name), TX_INHERIT);
		}
		~ThreadXMutex() override
		{
			tx_mutex_delete(&m_mutex);
		}

		void lock() noexcept override
		{
			tx_mutex_get(&m_mutex, TX_WAIT_FOREVER);
		}

		bool try_lock() noexcept override
		{
			UINT status = tx_mutex_get(&m_mutex, 0);
			return status == TX_SUCCESS;
		}

		bool try_lock_for(std::chrono::milliseconds timeout) noexcept override
		{
			ULONG ticks = static_cast<ULONG>((timeout.count() * TX_TIMER_TICKS_PER_SECOND) / 1000);
			UINT status = tx_mutex_get(&m_mutex, ticks);
			return status == TX_SUCCESS;
		}

		void unlock() noexcept override
		{
			tx_mutex_put(&m_mutex);
		}

	  private:
		TX_MUTEX m_mutex;
	};
} // namespace Rtos::ThreadXWrapper