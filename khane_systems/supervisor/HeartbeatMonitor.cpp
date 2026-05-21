#include "HeartbeatMonitor.hpp"

namespace khane::supervisor {

HeartbeatMonitor::HeartbeatMonitor() : system_healthy_(true) {}

void HeartbeatMonitor::register_module(uint8_t module_id, uint32_t timeout_ms) {
    modules_[module_id] = {0, timeout_ms, true};
}

void HeartbeatMonitor::process_heartbeat(uint8_t module_id, uint32_t tick) {
    if (modules_.find(module_id) != modules_.end()) {
        modules_[module_id].last_heartbeat_tick = tick;
        modules_[module_id].is_healthy = true;
    }
}

void HeartbeatMonitor::check_system_health(uint32_t current_tick) {
    system_healthy_ = true;
    for (auto& [id, status] : modules_) {
        if ((current_tick - status.last_heartbeat_tick) > status.timeout_threshold) {
            status.is_healthy = false;
            system_healthy_ = false;
        }
    }
}

bool HeartbeatMonitor::is_system_healthy() const {
    return system_healthy_;
}

} // namespace khane::supervisor
