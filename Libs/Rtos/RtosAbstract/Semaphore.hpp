// rtos/abstract/Semaphore.hpp
#pragma once
#include <cstdint>

namespace Rtos::Abstract {

class ISemaphore {
public:
    virtual ~ISemaphore() = default;
    virtual void give() noexcept = 0;
    // timeout in milliseconds
    virtual bool take(uint32_t timeout_ms) noexcept = 0;
    virtual bool take_from_isr() noexcept = 0;
};

} // namespace Rtos::Abstract
