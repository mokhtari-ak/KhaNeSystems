#include "IMessageQueue.hpp"
#include "InternalCommPolicies.hpp"
#include <functional>
#include <memory>
#include <vector>
#include <concepts>

template <TxMessageType T>
class Topic
{
  public:
	struct Subscriber
	{
		std::unique_ptr<IMessageQueue<T>> queue;
		std::function<bool(const T &)> filter;
	};

	void addSubscriber(std::unique_ptr<IMessageQueue<T>> q,
					   std::function<bool(const T &)> filter)
	{
		subscribers.push_back({std::move(q), filter});
	}

	void publish(const T &msg, unsigned long wait_option = 0)
	{
		for (auto &sub : subscribers)
		{
			if (sub.filter(msg))
			{
				sub.queue->send(msg, wait_option);
			}
		}
	}

	bool receive(size_t subscriberIndex, T &msg, unsigned long wait_option = 0)
	{
		if (subscriberIndex >= subscribers.size())
			return false;
		return subscribers[subscriberIndex].queue->receive(msg, wait_option);
	}

  private:
	std::vector<Subscriber> subscribers;
};