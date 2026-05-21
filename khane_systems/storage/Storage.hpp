#pragma once

#include <cstdint>
#include <array>
#include "libs/stm32_wrapper/HardwareAccessLayer/SdCardRawDriver.hpp"

namespace khane::storage {

/**
 * @brief Storage module for non-blocking raw sector logging.
 * Uses double-buffering to ensure flight control loops are never blocked.
 */
class Storage {
public:
    static constexpr size_t SECTOR_SIZE = 512;
    static constexpr size_t BUFFER_SIZE = SECTOR_SIZE;

    Storage(hw::ISdCardRawDriver& driver);

    // Logs data into the active buffer. If full, triggers background write.
    bool log(const uint8_t* data, size_t size);

    // Should be called in a background task (e.g., RTOS idle or low-priority task)
    void process();

private:
    hw::ISdCardRawDriver& driver_;
    
    std::array<uint8_t, BUFFER_SIZE> buffer_a_;
    std::array<uint8_t, BUFFER_SIZE> buffer_b_;
    
    uint8_t* active_buffer_;
    uint8_t* write_buffer_;
    size_t offset_ = 0;
    bool write_pending_ = false;
    uint32_t current_sector_ = 0;
};

} // namespace khane::storage
