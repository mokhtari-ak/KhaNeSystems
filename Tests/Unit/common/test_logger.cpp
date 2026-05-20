#include <gtest/gtest.h>
#include <common/Inc/logger.hpp>
#include <type_traits>
#include <thread>
#include <vector>

using namespace khane::common;

TEST(LoggerTypesTest, LogEntryIsPOD) {
    EXPECT_TRUE(std::is_standard_layout_v<log_entry>);
    EXPECT_TRUE(std::is_trivially_copyable_v<log_entry>);
}

TEST(LoggerTest, BasicLogging) {
    // Log something general
    KHANE_LOG_INFO(1, 100, 42);
    
    log_entry entry;
    bool success = logger::pop(entry);
    
    EXPECT_TRUE(success);
    EXPECT_EQ(entry.level, log_level::info);
    EXPECT_EQ(static_cast<uint16_t>(entry.module_id), 1);
    EXPECT_EQ(entry.msg_id, 100);
    EXPECT_EQ(entry.arg, 42);
    // Timestamp might be 0 if the host clock is very early, but usually > 0
}

TEST(LoggerTest, DefaultArgumentMacro) {
    // Test that variadic macro works with default arg
    KHANE_LOG_INFO(1, 101);
    
    log_entry entry;
    bool success = logger::pop(entry);
    
    EXPECT_TRUE(success);
    EXPECT_EQ(entry.msg_id, 101);
    EXPECT_EQ(entry.arg, 0);
}

TEST(LoggerTest, CriticalLogPriority) {
    // Log info then critical
    KHANE_LOG_INFO(1, 100, 1);
    KHANE_LOG_CRITICAL(1, 200, 2);
    
    log_entry entry;
    // Pop should give critical first because it's checked first in pop()
    bool success = logger::pop(entry);
    
    EXPECT_TRUE(success);
    EXPECT_EQ(entry.level, log_level::critical);
    EXPECT_EQ(entry.msg_id, 200);
    
    // Next pop should give info
    success = logger::pop(entry);
    EXPECT_TRUE(success);
    EXPECT_EQ(entry.level, log_level::info);
    EXPECT_EQ(entry.msg_id, 100);
}

TEST(LoggerTest, OuroborosPolicy) {
    // Fill general buffer (128)
    for (int i = 0; i < 130; ++i) {
        KHANE_LOG_DEBUG(1, i, 0);
    }
    
    log_entry entry;
    logger::pop(entry);
    // The first one should be msg_id 2 if it's Ouroboros (130 - 128 = 2 dropped)
    // Wait, Ouroboros overwrites oldest.
    // If capacity is 128, and we push 130:
    // 0..127 fill. 128 overwrites 0. 129 overwrites 1.
    // So head should be at index 2.
    EXPECT_EQ(entry.msg_id, 2);
}

TEST(LoggerTest, ThreadSafety) {
    const int num_threads = 4;
    const int logs_per_thread = 50;
    std::vector<std::thread> threads;
    
    // Clear buffers first (by popping everything)
    log_entry dummy;
    while(logger::pop(dummy));

    for (int t = 0; t < num_threads; ++t) {
        threads.emplace_back([t, logs_per_thread]() {
            for (int i = 0; i < logs_per_thread; ++i) {
                KHANE_LOG_INFO(t, i, 0);
            }
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    int count = 0;
    while(logger::pop(dummy)) {
        count++;
    }
    
    EXPECT_EQ(count, num_threads * logs_per_thread);
}
