#pragma once

#include "../Drivers/ISensor.hpp"
#include "../Drivers/IBus.hpp"
#include <expected>
#include <span>

/**
 * @brief Données GPS.
 */
struct GpsData {
    double latitude;
    double longitude;
    float altitude;
    uint32_t timestamp;
};

/**
 * @brief Driver pour module GPS (UBLOX/NMEA).
 * Implémente le concept Sensor.
 */
class GpsDriver {
public:
    using Data = GpsData;

    explicit GpsDriver(IBus& bus) : bus_(bus) {}

    bool init() {
        // Configuration UART/I2C pour le mode binaire
        return true;
    }

    std::expected<size_t, SensorError> read_batch(std::span<Data> buffer) {
        if (buffer.empty()) return 0;
        // Lecture NMEA/UBX et parsing
        return 0;
    }

    bool is_healthy() const { return true; }
    uint64_t get_last_timestamp_us() const { return 0; }
    uint32_t get_update_rate_hz() const { return 5; } // GPS tourne souvent à 5-10Hz

private:
    IBus& bus_;
};
