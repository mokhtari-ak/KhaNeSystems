#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "tx_api.h"

#define APP_STACK_SIZE 512
#define TX_APP_MEM_POOL_SIZE 3 * 512
//#define APP_BYTE_POOL_SIZE (2 * 1024)

static UCHAR tx_byte_pool_buffer[TX_APP_MEM_POOL_SIZE];
static TX_BYTE_POOL tx_app_byte_pool;

void Error_Handler(void);

/*
 * @brief ThreadX Application define function.
 * @note Déclaration Config d'un thread : using TaskConfigA = ThreadStaticConfig<[]() { while (1) { tx_thread_sleep(10); } }, 1, "TaskA", 5, 5, 2048, true>;.
 * @note Déclaration d'un thread : std::unique_ptr<ThreadXStaticThread<TaskConfigA>> g_thread_A;
 * @note Initialisation du thread : high_prio_thread = std::make_unique<ThreadXStaticThread<HighPrioConfig>>();
 **/
void tx_application_define(void *first_unused_memory)
{
	VOID *memory_ptr;

	if (tx_byte_pool_create(&tx_app_byte_pool, "Tx App memory pool", tx_byte_pool_buffer, TX_APP_MEM_POOL_SIZE) != TX_SUCCESS)
	{
		Error_Handler();
	}
}

#ifdef __cplusplus
}
#endif