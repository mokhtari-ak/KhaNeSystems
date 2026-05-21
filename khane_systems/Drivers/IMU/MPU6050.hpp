#pragma once

#include "../ISensor.hpp"
#include "../IBus.hpp"
#include <expected>
#include <span>

/**
 * @brief Structure de données spécifique au capteur MPU6050.
 * POD (Plain Old Data) pour garantir la performance et le déterminisme.
 */
struct GyroData {
    float x, y, z;      // rad/s
    uint32_t timestamp; // Timestamp local
};

/**
 * @brief Driver pour le MPU6050.
 * Implémente le concept Sensor.
 */
class Mpu6050Driver {
public:
    using Data = GyroData;

    explicit Mpu6050Driver(IBus& bus) : bus_(bus) {}

    /**
     * @brief Initialise le capteur via le bus injecté.
     */
    bool init() {
        // Logique de configuration des registres (SMPLRT_DIV, CONFIG, GYRO_CONFIG, etc.)
        return true; 
    }

    /**
     * @brief Lit la FIFO du MPU6050 et remplit le buffer fourni.
     */
    std::expected<size_t, SensorError> read_batch(std::span<Data> buffer) {
        if (buffer.empty()) return 0;
        
        // Logique :
        // 1. Lire le registre de taille FIFO (FIFO_COUNTH/L)
        // 2. Transférer les données via bus_ (SPI/I2C)
        // 3. Parser les octets bruts en GyroData
        
        return 0; // Nombre d'échantillons lus
    }

    bool is_healthy() const {
        // Vérifier WHO_AM_I ou présence d'interruptions
        return true;
    }

    uint64_t get_last_timestamp_us() const { return last_timestamp_us_; }
    uint32_t get_update_rate_hz() const { return 1000; }

private:
    IBus& bus_;
    uint64_t last_timestamp_us_{0};
};
