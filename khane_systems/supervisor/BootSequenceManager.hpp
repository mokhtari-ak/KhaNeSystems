#pragma once

#include <vector>
#include <functional>
#include <string>

namespace khane::supervisor {

enum class BootStage {
    BSP_INIT,
    CLOCK_CONFIG,
    DRIVERS_INIT,
    EKF_READY,
    READY_TO_ARM
};

class BootSequenceManager {
public:
    using ValidationFunc = std::function<bool()>;

    void add_stage(BootStage stage, ValidationFunc validation);
    bool run_next_stage();
    BootStage get_current_stage() const { return current_stage_; }
    bool is_boot_complete() const { return boot_complete_; }

private:
    std::vector<std::pair<BootStage, ValidationFunc>> stages_;
    BootStage current_stage_ = BootStage::BSP_INIT;
    bool boot_complete_ = false;
};

} // namespace khane::supervisor
