#pragma once
#include "InternalCommPolicies.hpp"
template <TxMessageType T>
class IMessageQueue
{
  public:
	virtual ~IMessageQueue() = default;
	virtual bool send(const T &msg, unsigned long wait_option) = 0;
	virtual bool receive(T &msg, unsigned long wait_option) = 0;
};