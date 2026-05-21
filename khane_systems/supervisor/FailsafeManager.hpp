#pragma once

#include <cstdint>

namespace khane::supervisor {

enum class FailsafeLevel {
    NONE = 0,
    WARNING,
    CRITICAL,
    EMERGENCY
};

class FailsafeManager {
public:
    FailsafeManager();

    // Set failure severity and trigger corresponding action
    void trigger_failsafe(FailsafeLevel level);

    FailsafeLevel get_current_level() const { return current_level_; }

private:
    FailsafeLevel current_level_;
    void execute_action(FailsafeLevel level);
};

} // namespace khane::supervisor
