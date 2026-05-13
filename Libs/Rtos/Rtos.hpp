// rtos/Rtos.hpp
#pragma once
#include "RtosConfig.hpp"
#include <cstdint>

#if defined(USE_FREERTOS)
  #include "FreeRtosMutex.hpp"
  #include "FreeRtosSemaphore.hpp"
  #include "FreeRtosThread.hpp"
  #include "FreeRtosQueue.hpp"
  namespace Rtos {
	  using Mutex = Rtos::FreeRtosWrapper::FreeRtosMutex;
	  using Semaphore = Rtos::FreeRtosWrapper::FreeRtosSemaphore;
	  using Thread = Rtos::FreeRtosWrapper::FreeRtosThread;
	  using Queue = Rtos::FreeRtosWrapper::FreeRtosQueue;
  }
#elif defined(USE_THREADX)
#include "ThreadXMutex.hpp"
#include "ThreadXSemaphore.hpp"
#include "ThreadXThread.hpp"
#include "ThreadXQueue.hpp"
  namespace Rtos {
	  
	  template <const char *name = nullptr>
	  using Mutex = Rtos::ThreadXWrapper::ThreadXMutex<name>;

	  template <uint16_t initial_count = 0, uint16_t max_count = 1, const char *name = nullptr>
	  using Semaphore = Rtos::ThreadXWrapper::ThreadXSemaphore<initial_count, max_count, name>;
	  
	  template <Rtos::Abstract::ThreadStaticConfigConcept config>
	  using Thread = Rtos::ThreadXWrapper::ThreadXThread<config>;

	  template <uint16_t maxQueueLength, uint16_t itemSize, const char *name = nullptr>
	  using Queue = Rtos::ThreadXWrapper::ThreadXQueue<maxQueueLength, itemSize, name>;
  }
#else
  #error "No RTOS defined. Define USE_FREERTOS or USE_THREADX in RtosConfig.hpp"
#endif
