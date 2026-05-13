// rtos/abstract/Thread.hpp
#pragma once
#include <functional>
#include <cstdint>
#include <vector>
#include "ThreadTypes.hpp"

namespace Rtos::Abstract {
	class IThread
	{
	  public:
		virtual ~IThread() = default;
		virtual void start() noexcept = 0;
		virtual void stop() noexcept = 0;
	  protected:
		virtual void create() = 0;
		virtual void destroy() = 0;
	};

} // namespace Rtos::Abstract
