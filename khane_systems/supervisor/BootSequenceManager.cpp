#include "BootSequenceManager.hpp"

namespace khane::supervisor {

void BootSequenceManager::add_stage(BootStage stage, ValidationFunc validation) {
    stages_.push_back({stage, validation});
}

bool BootSequenceManager::run_next_stage() {
    for (auto& stage_pair : stages_) {
        if (stage_pair.first == current_stage_) {
            if (stage_pair.second()) {
                if (current_stage_ == BootStage::READY_TO_ARM) {
                    boot_complete_ = true;
                } else {
                    current_stage_ = static_cast<BootStage>(static_cast<int>(current_stage_) + 1);
                }
                return true;
            }
            return false;
        }
    }
    return false;
}

} // namespace khane::supervisor
