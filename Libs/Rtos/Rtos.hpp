// rtos/Rtos.hpp
#pragma once
#include "RtosConfig.hpp"

#if defined(USE_FREERTOS)
  #include "FreeRtosWrapper/FreeRtosMutex.hpp"
  #include "FreeRtosWrapper/FreeRtosSemaphore.hpp"
  #include "FreeRtosWrapper/FreeRtosThread.hpp"
  #include "FreeRtosWrapper/FreeRtosQueue.hpp"
  namespace Rtos {
    using Mutex = FreeRTOS::Mutex;
    using Semaphore = FreeRTOS::Semaphore;
    using Thread = FreeRTOS::Thread;
    using Queue = FreeRTOS::Queue;
  }
#elif defined(USE_THREADX)
  #include "threadx/Mutex.hpp"
  #include "threadx/Semaphore.hpp"
  #include "threadx/Thread.hpp"
  #include "threadx/Queue.hpp"
  namespace Rtos {
    using Mutex = ThreadX::Mutex;
    using Semaphore = ThreadX::Semaphore;
    using Thread = ThreadX::Thread;
    using Queue = ThreadX::Queue;
  }
#else
  #error "No RTOS defined. Define USE_FREERTOS or USE_THREADX in RtosConfig.hpp"
#endif
