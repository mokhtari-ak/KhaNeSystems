// rtos/abstract/Semaphore.hpp
#pragma once
#include <chrono>

namespace Rtos::Abstract {

class ISemaphore {
public:
    virtual ~ISemaphore() = default;
    virtual void give() noexcept = 0;
    virtual bool take(std::chrono::milliseconds timeout) noexcept = 0;
    virtual bool take_from_isr() noexcept = 0;
};

} // namespace Rtos::Abstract
