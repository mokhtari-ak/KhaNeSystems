#pragma once
#include "Queue.hpp"
#include "tx_api.h"
#include <cstddef>
#include <cstdint>

namespace Rtos::ThreadXWrapper
{
	template <uint16_t maxQueueLength, uint16_t itemSize, const char *name = nullptr>
	class ThreadXQueue : public Rtos::Abstract::IQueue
	{
	  public:
		ThreadXQueue()
			: m_itemSize(itemSize), m_maxQueueLength(maxQueueLength)
		{
			// ThreadX queue requires a buffer: each message is a multiple of ULONG
			UINT messageSizeInULONG = (itemSize + sizeof(ULONG) - 1) / sizeof(ULONG);
			m_messageSizeInULONG = messageSizeInULONG;
			m_queueBuffer = new ULONG[maxQueueLength * messageSizeInULONG];
			tx_queue_create(&m_queue, (CHAR *)(name ? name : "ThreadXQueue"), messageSizeInULONG, m_queueBuffer, maxQueueLength * messageSizeInULONG * sizeof(ULONG));
		}

		~ThreadXQueue() override
		{
			tx_queue_delete(&m_queue);
			delete[] m_queueBuffer;
		}

		bool send(const void *item, size_t size, uint32_t timeoutMs) noexcept override
		{
			if (size != m_itemSize)
				return false;
			ULONG ticks = (timeoutMs == 0xFFFFFFFF) ? TX_WAIT_FOREVER : (timeoutMs * TX_TIMER_TICKS_PER_SECOND) / 1000;
			return tx_queue_send(&m_queue, const_cast<void *>(item), ticks) == TX_SUCCESS;
		}

		bool receive(void *outItem, size_t size, uint32_t timeoutMs) noexcept override
		{
			if (size != m_itemSize)
				return false;
			ULONG ticks = (timeoutMs == 0xFFFFFFFF) ? TX_WAIT_FOREVER : (timeoutMs * TX_TIMER_TICKS_PER_SECOND) / 1000;
			return tx_queue_receive(&m_queue, outItem, ticks) == TX_SUCCESS;
		}

	  private:
		TX_QUEUE m_queue;
		ULONG *m_queueBuffer;
		UINT m_itemSize;
		UINT m_maxQueueLength;
		UINT m_messageSizeInULONG;
	};

} // namespace Rtos::ThreadXWrapper