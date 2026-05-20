#include <gtest/gtest.h>
#include "khane_systems/storage/Storage.hpp"

class MockSdDriver : public khane::hw::ISdCardRawDriver {
public:
    bool initialize() override { return true; }
    bool read_block(uint32_t addr, uint8_t* buf) override { return true; }
    bool write_block(uint32_t addr, const uint8_t* buf) override {
        write_count++;
        return true;
    }
    bool is_ready() override { return true; }
    int write_count = 0;
};

TEST(StorageTest, BufferSwapWorks) {
    MockSdDriver driver;
    khane::storage::Storage storage(driver);

    uint8_t data[256];
    memset(data, 0xAA, 256);

    // Fill first half
    EXPECT_TRUE(storage.log(data, 256));
    // Fill second half, triggers buffer swap
    EXPECT_TRUE(storage.log(data, 256));
    
    storage.process();
    EXPECT_EQ(driver.write_count, 1);
}
