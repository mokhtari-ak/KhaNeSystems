#pragma once

#include "stm32f4xx_hal.h"
#include <cstdint>
#include <optional>

namespace khane::hw {

/**
 * @brief Interface for Raw SD Card access
 */
class ISdCardRawDriver {
public:
    virtual ~ISdCardRawDriver() = default;
    virtual bool initialize() = 0;
    virtual bool read_block(uint32_t block_address, uint8_t* buffer) = 0;
    virtual bool write_block(uint32_t block_address, const uint8_t* buffer) = 0;
    virtual bool is_ready() = 0;
};

/**
 * @brief Concrete implementation using STM32 HAL SDMMC
 */
class SdCardRawDriver : public ISdCardRawDriver {
public:
    SdCardRawDriver(SD_HandleTypeDef* hsd);
    
    bool initialize() override;
    bool read_block(uint32_t block_address, uint8_t* buffer) override;
    bool write_block(uint32_t block_address, const uint8_t* buffer) override;
    bool is_ready() override;

private:
    SD_HandleTypeDef* hsd_;
};

} // namespace khane::hw
