#include <gtest/gtest.h>
#include "khane_systems/supervisor/BootSequenceManager.hpp"

TEST(BootSequenceManagerTest, SequentialBootWorks) {
    khane::supervisor::BootSequenceManager manager;
    
    manager.add_stage(khane::supervisor::BootStage::BSP_INIT, [](){ return true; });
    manager.add_stage(khane::supervisor::BootStage::CLOCK_CONFIG, [](){ return true; });

    EXPECT_TRUE(manager.run_next_stage());
    EXPECT_EQ(manager.get_current_stage(), khane::supervisor::BootStage::CLOCK_CONFIG);
    
    EXPECT_TRUE(manager.run_next_stage());
}

TEST(BootSequenceManagerTest, FailsOnValidationError) {
    khane::supervisor::BootSequenceManager manager;
    
    manager.add_stage(khane::supervisor::BootStage::BSP_INIT, [](){ return false; });

    EXPECT_FALSE(manager.run_next_stage());
    EXPECT_EQ(manager.get_current_stage(), khane::supervisor::BootStage::BSP_INIT);
}
