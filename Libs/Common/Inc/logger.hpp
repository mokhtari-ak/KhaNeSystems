#pragma once

#include "logger_types.hpp"
#include <ouroboros_buffer.hpp>
#include <rtos_control.hpp>
#include <cstddef>

namespace khane::common {

/**
 * @brief Thread-safe, non-blocking logger using dual circular buffers.
 * 
 * This logger separates critical logs from general logs to ensure that 
 * high-frequency debug data doesn't evict critical errors.
 */
class logger {
public:
    // Capacities for the buffers (statically allocated)
    static constexpr size_t CRITICAL_CAPACITY = 32;
    static constexpr size_t GENERAL_CAPACITY = 128;

    /**
     * @brief Log a binary message. Thread-safe and non-blocking (Ouroboros policy).
     * 
     * @param level Severity level
     * @param module_id Originating module ID
     * @param msg_id Message template identifier
     * @param arg Optional 32-bit payload
     */
    static void log(log_level level, uint16_t module_id, uint16_t msg_id, uint32_t arg = 0) noexcept;

    /**
     * @brief Convenience helpers for different levels.
     */
    static void debug(uint16_t module_id, uint16_t msg_id, uint32_t arg = 0) noexcept { log(log_level::debug, module_id, msg_id, arg); }
    static void info(uint16_t module_id, uint16_t msg_id, uint32_t arg = 0) noexcept { log(log_level::info, module_id, msg_id, arg); }
    static void warning(uint16_t module_id, uint16_t msg_id, uint32_t arg = 0) noexcept { log(log_level::warning, module_id, msg_id, arg); }
    static void error(uint16_t module_id, uint16_t msg_id, uint32_t arg = 0) noexcept { log(log_level::error, module_id, msg_id, arg); }
    static void critical(uint16_t module_id, uint16_t msg_id, uint32_t arg = 0) noexcept { log(log_level::critical, module_id, msg_id, arg); }

    /**
     * @brief Flush logs to a consumer (e.g., storage task).
     * 
     * @param out_entry Destination for the popped entry.
     * @param only_critical If true, only check the critical buffer.
     * @return true if an entry was popped.
     */
    static bool pop(log_entry& out_entry, bool only_critical = false) noexcept;

private:
    using buffer_t = communication::event_bus::ouroboros_buffer<log_entry, GENERAL_CAPACITY>;
    using crit_buffer_t = communication::event_bus::ouroboros_buffer<log_entry, CRITICAL_CAPACITY>;

    // Use a helper function for static members to avoid "Static Initialization Order Fiasco"
    struct context {
        crit_buffer_t critical_buffer;
        buffer_t general_buffer;
        rtos::static_mutex mutex;
    };

    static context& get_context() noexcept;
};

} // namespace khane::common

// Helper macros for easier logging - using variadic macros to support default arguments
#define KHANE_LOG_DEBUG(mod, msg, ...) khane::common::logger::debug(mod, msg, ##__VA_ARGS__)
#define KHANE_LOG_INFO(mod, msg, ...) khane::common::logger::info(mod, msg, ##__VA_ARGS__)
#define KHANE_LOG_WARN(mod, msg, ...) khane::common::logger::warning(mod, msg, ##__VA_ARGS__)
#define KHANE_LOG_ERROR(mod, msg, ...) khane::common::logger::error(mod, msg, ##__VA_ARGS__)
#define KHANE_LOG_CRITICAL(mod, msg, ...) khane::common::logger::critical(mod, msg, ##__VA_ARGS__)
