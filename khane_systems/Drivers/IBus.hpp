#pragma once

#include <cstdint>
#include <cstddef>
#include <expected>

/**
 * @brief Erreurs standardisées pour les communications bus.
 */
enum class BusError {
    Timeout,
    TransferFailed,
    InvalidAddress
};

/**
 * @brief Interface abstraite pour les bus de communication (SPI, I2C).
 * 
 * Permet aux drivers d'être agnostiques vis-à-vis du matériel (STM32 HAL vs Mock).
 */
class IBus {
public:
    virtual ~IBus() = default;

    /**
     * @brief Lit une série d'octets depuis une adresse registre.
     */
    virtual std::expected<size_t, BusError> read(uint8_t reg_addr, uint8_t* data, size_t length) = 0;

    /**
     * @brief Écrit une série d'octets vers une adresse registre.
     */
    virtual std::expected<size_t, BusError> write(uint8_t reg_addr, const uint8_t* data, size_t length) = 0;
};
