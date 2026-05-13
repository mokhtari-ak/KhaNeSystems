#pragma once

// RTOS backend must be selected from CMake compile definitions.
#if defined(USE_FREERTOS)
#include "FreeRTOS.h"
#elif defined(USE_THREADX)
#include "tx_api.h"
#else
#error "No RTOS defined. Define USE_FREERTOS or USE_THREADX from build system."
#endif
