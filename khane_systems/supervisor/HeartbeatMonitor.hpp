#pragma once

#include <cstdint>
#include <map>
#include <vector>

namespace khane::supervisor {

struct ModuleStatus {
    uint32_t last_heartbeat_tick;
    uint32_t timeout_threshold;
    bool is_healthy;
};

class HeartbeatMonitor {
public:
    HeartbeatMonitor();

    // Register a module to monitor
    void register_module(uint8_t module_id, uint32_t timeout_ms);

    // Call when a heartbeat is received
    void process_heartbeat(uint8_t module_id, uint32_t tick);

    // Run monitoring (checks for timeouts)
    void check_system_health(uint32_t current_tick);

    bool is_system_healthy() const;

private:
    std::map<uint8_t, ModuleStatus> modules_;
    bool system_healthy_;
};

} // namespace khane::supervisor
