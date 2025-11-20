#ifndef THREADX_THREAD_HPP
#define THREADX_THREAD_HPP

#include "tx_api.h"

namespace ThreadXWrapper
{

class ThreadXThread
{
public:
    ThreadXThread(
        CHAR* name,
        void (*entry_function)(ULONG),
        ULONG entry_input,
        VOID* stack_start,
        ULONG stack_size,
        UINT priority,
        UINT preemption_threshold,
        ULONG time_slice,
        UINT auto_start,
        CHAR* thread_info_ptr
    );

    ~ThreadXThread();

    UINT resume();
    UINT suspend();
    UINT terminate();
    UINT sleep(ULONG ticks);

private:
    TX_THREAD m_thread;
};

} // namespace ThreadXWrapper

#endif // THREADX_THREAD_HPP
