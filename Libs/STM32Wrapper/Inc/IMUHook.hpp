#pragma once
#include "Common/SensorTypes.hpp"
#include <cmath>

namespace Mocks {

    /**
     * @brief Hook de simulation pour l'IMU (Mock).
     * Permet de simuler des données pour AirSim.
     */
    class IMUHook : public Sensors::SensorInterface<Sensors::IMUData> {
    public:
        IMUHook() noexcept = default;

        Sensors::IMUData update() noexcept override {
            m_cycle_count++;
            
            // Simulation d'une oscillation sinus pour le test
            float time = static_cast<float>(m_cycle_count) * 0.01f;
            
            return Sensors::IMUData{
                .accel_x = 0.0f,
                .accel_y = 0.0f,
                .accel_z = 9.81f + 0.1f * std::sin(time), // Gravité + bruit simulé
                .gyro_x = 0.02f * std::cos(time),
                .gyro_y = 0.0f,
                .gyro_z = 0.0f,
                .timestamp_us = m_cycle_count * 1000,
                .status_flags = Sensors::IMUData::FLAG_VALID
            };
        }

        bool calibrate() noexcept override {
            return true; // Calibration toujours réussie en simulation
        }

    private:
        uint64_t m_cycle_count{0};
    };

} // namespace Mocks
