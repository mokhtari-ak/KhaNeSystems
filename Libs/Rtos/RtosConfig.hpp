#pragma once

// Define one of these in build system or uncomment here:
#define USE_FREERTOS
// #define USE_THREADX

#if defined(USE_FREERTOS)
#include "FreeRTOS.h"
#elif defined(USE_THREADX)
#include "tx_api.h""
#else
#error "No RTOS defined. Define USE_FREERTOS or USE_THREADX in RtosConfig.hpp"
#endif
