#pragma once

#include <cstdint>
#include "../../internal_communication/Inc/event_bus.hpp"

namespace khane::common {

using communication::event_bus::message_priority;

/**
 * @brief Log levels following the architectural requirements.
 */
enum class log_level : uint8_t {
    debug = 0,
    info,
    warning,
    error,
    critical
};

/**
 * @brief Binary log entry optimized for RAM storage and raw sector storage.
 */
struct log_entry {
    uint64_t timestamp;  ///< Monotonic system time in microseconds
    log_level level;     ///< Severity level
    uint8_t module_id;   ///< ID of the originating module
    uint16_t msg_id;     ///< Message identifier (stub/template ID)
    uint32_t arg;        ///< Optional 32-bit argument or packed payload
    message_priority priority = message_priority::normal;
} __attribute__((packed, aligned(8)));

} // namespace khane::common
