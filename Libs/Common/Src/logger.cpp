#include "Inc/logger.hpp"

namespace khane::common {

logger::context& logger::get_context() noexcept {
    static context ctx;
    return ctx;
}

void logger::log(log_level level, uint16_t module_id, uint16_t msg_id, uint32_t arg) noexcept {
    log_entry entry{
        .timestamp = rtos::kernel::get_tick_count(),
        .level = level,
        .module_id = static_cast<uint8_t>(module_id), // module_id in log_entry is uint8_t
        .msg_id = msg_id,
        .arg = arg
    };

    auto& ctx = get_context();

    // Non-blocking: use try_lock to avoid priority inversion and handle ISR
    // In a real ISR, we might want a lock-free approach, but for now, 
    // a non-blocking try_lock prevents deadlock.
    if (ctx.mutex.try_lock(0)) {
        if (level == log_level::critical || level == log_level::error) {
            ctx.critical_buffer.push(entry);
        } else {
            ctx.general_buffer.push(entry);
        }
        ctx.mutex.unlock();
    }
}

bool logger::pop(log_entry& out_entry, bool only_critical) noexcept {
    auto& ctx = get_context();
    
    // pop is usually called from a low-priority task, so blocking is okay
    ctx.mutex.lock();
    bool success = ctx.critical_buffer.pop(out_entry);
    if (!success && !only_critical) {
        success = ctx.general_buffer.pop(out_entry);
    }
    ctx.mutex.unlock();
    return success;
}

} // namespace khane::common
