// rtos/abstract/Mutex.hpp
#pragma once
#include <chrono>
#include <cstdint>

namespace Rtos::Abstract {

class IMutex {
public:
    virtual ~IMutex() = default;
    virtual void lock() noexcept = 0;
    virtual bool try_lock() noexcept = 0;
    virtual bool try_lock_for(std::chrono::milliseconds timeout) noexcept = 0;
    virtual void unlock() noexcept = 0;
};

class LockGuard {
    IMutex& m_;
public:
    explicit LockGuard(IMutex& m) noexcept : m_(m) { m_.lock(); }
    ~LockGuard() noexcept { m_.unlock(); }
    LockGuard(const LockGuard&) = delete;
    LockGuard& operator=(const LockGuard&) = delete;
};

} // namespace Rtos::Abstract
