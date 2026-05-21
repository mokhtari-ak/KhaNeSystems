#if !defined(USE_THREADX)
#error "ThreadXMessageQueue requires USE_THREADX backend."
#endif

#include "tx_api.h"
#include "IMessageQueue.hpp"
#include "InternalCommPolicies.hpp"
#include <array>
#include <concepts>
#include <stdexcept>

template <TxMessageType T, size_t Capacity>
class ThreadXMessageQueue : public IMessageQueue<T>
{
  public:
	ThreadXMessageQueue(const char *name)
	{
		buffer.fill(0);
		UINT status = tx_queue_create(&queue,
									  const_cast<char *>(name),
									  sizeof(T) / sizeof(ULONG),
									  buffer.data(),
									  buffer.size() * sizeof(ULONG));
		if (status != TX_SUCCESS)
		{
			throw std::runtime_error("Failed to create queue");
		}
	}

	~ThreadXMessageQueue()
	{
		tx_queue_delete(&queue);
	}

	bool send(const T &msg, unsigned long wait_option) override
	{
		return tx_queue_send(&queue, (VOID *)&msg, wait_option) == TX_SUCCESS;
	}

	bool receive(T &msg, unsigned long wait_option) override
	{
		return tx_queue_receive(&queue, (VOID *)&msg, wait_option) == TX_SUCCESS;
	}

  private:
	TX_QUEUE queue;
	std::array<ULONG, Capacity * (sizeof(T) / sizeof(ULONG))> buffer;
};
