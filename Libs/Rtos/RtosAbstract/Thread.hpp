// rtos/abstract/Thread.hpp
#pragma once
#include <functional>
#include <cstdint>

namespace Rtos::Abstract {

class IThread {
public:
    virtual ~IThread() = default;
    virtual bool start(std::function<void()> entry, const char* name = nullptr,
                       uint32_t stackSizeBytes = 1024, int priority = 1) noexcept = 0;
    virtual void join() noexcept = 0;
    virtual bool isRunning() const noexcept = 0;
};

} // namespace Rtos::Abstract
