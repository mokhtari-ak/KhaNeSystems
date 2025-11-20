// rtos/abstract/Queue.hpp
#pragma once
#include <cstddef>

namespace Rtos::Abstract {

class IQueue {
public:
    virtual ~IQueue() = default;
    virtual bool send(const void* item, size_t size, uint32_t timeoutMs) noexcept = 0;
    virtual bool receive(void* outItem, size_t size, uint32_t timeoutMs) noexcept = 0;
};

} // namespace Rtos::Abstract
