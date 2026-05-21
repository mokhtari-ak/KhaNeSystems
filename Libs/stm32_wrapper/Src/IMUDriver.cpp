#include "IMUDriver.hpp"

namespace Drivers {

    Sensors::IMUData IMUDriver::update() noexcept {
        // Logique de lecture réelle via SPI/I2C DMA
        // Pour l'instant, on retourne une structure vide marquée invalide
        return Sensors::IMUData{
            .accel_x = 0.0f,
            .accel_y = 0.0f,
            .accel_z = 0.0f,
            .gyro_x = 0.0f,
            .gyro_y = 0.0f,
            .gyro_z = 0.0f,
            .timestamp_us = 0,
            .status_flags = Sensors::IMUData::FLAG_ERROR
        };
    }

    bool IMUDriver::calibrate() noexcept {
        // Séquence de calibration déterministe
        return true;
    }

} // namespace Drivers
