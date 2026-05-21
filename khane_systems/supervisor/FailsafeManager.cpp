#include "FailsafeManager.hpp"
#include <iostream> // For demonstration, will be replaced by event bus logging

namespace khane::supervisor {

FailsafeManager::FailsafeManager() : current_level_(FailsafeLevel::NONE) {}

void FailsafeManager::trigger_failsafe(FailsafeLevel level) {
    if (level > current_level_) {
        current_level_ = level;
        execute_action(level);
    }
}

void FailsafeManager::execute_action(FailsafeLevel level) {
    switch (level) {
        case FailsafeLevel::WARNING:
            // Log warning
            break;
        case FailsafeLevel::CRITICAL:
            // Trigger controlled landing
            break;
        case FailsafeLevel::EMERGENCY:
            // Emergency disarm/cutoff
            break;
        default:
            break;
    }
}

} // namespace khane::supervisor
