#include "SdCardRawDriver.hpp"

namespace khane::hw {

SdCardRawDriver::SdCardRawDriver(SD_HandleTypeDef* hsd) : hsd_(hsd) {}

bool SdCardRawDriver::initialize() {
    // HAL_SD_Init(hsd_);
    return true; // Simplified for skeleton
}

bool SdCardRawDriver::read_block(uint32_t block_address, uint8_t* buffer) {
    // return HAL_SD_ReadBlocks(hsd_, buffer, block_address, 1, 1000) == HAL_OK;
    return true; // Simplified for skeleton
}

bool SdCardRawDriver::write_block(uint32_t block_address, const uint8_t* buffer) {
    // return HAL_SD_WriteBlocks(hsd_, (uint8_t*)buffer, block_address, 1, 1000) == HAL_OK;
    return true; // Simplified for skeleton
}

bool SdCardRawDriver::is_ready() {
    return true;
}

} // namespace khane::hw
