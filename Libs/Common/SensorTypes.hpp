#pragma once
#include <cstdint>

namespace Sensors {

    /**
     * @brief Structure de données IMU normalisée.
     */
    struct IMUData {
        float accel_x;    // m/s^2
        float accel_y;
        float accel_z;
        float gyro_x;     // rad/s
        float gyro_y;
        float gyro_z;
        uint64_t timestamp_us;
        uint32_t status_flags;

        static constexpr uint32_t FLAG_VALID = 0x01;
        static constexpr uint32_t FLAG_TIMEOUT = 0x02;
        static constexpr uint32_t FLAG_ERROR = 0x04;
    };

    /**
     * @brief Données GNSS (Global Navigation Satellite System).
     */
    struct GNSSData {
        double latitude;   // degrés
        double longitude;  // degrés
        float altitude_msl; // m (Mean Sea Level)
        float vel_n;       // m/s (North)
        float vel_e;       // m/s (East)
        float vel_d;       // m/s (Down)
        float hdop;        // Dilution de précision horizontale
        uint8_t fix_type;  // 0: No fix, 3: 3D fix, etc.
        uint8_t num_sats;
        uint64_t timestamp_us;
        uint32_t status_flags;
    };

    /**
     * @brief Données Baromètre (Altimétrie).
     */
    struct BaroData {
        float pressure_pa;   // Pascal
        float temperature_c; // Celsius
        float altitude_m;    // Altitude relative ou absolue (m)
        uint64_t timestamp_us;
        uint32_t status_flags;
    };

    /**
     * @brief Données Air Speed (Vitesse air via tube Pitot).
     */
    struct AirSpeedData {
        float differential_pressure_pa; // Pascal
        float indicated_airspeed_ms;    // m/s
        float true_airspeed_ms;         // m/s
        uint64_t timestamp_us;
        uint32_t status_flags;
    };

    /**
     * @brief Interface générique pour les drivers de capteurs.
     */
    template<typename DataType>
    class SensorInterface {
    public:
        virtual ~SensorInterface() = default;
        virtual DataType update() noexcept = 0;
        virtual bool calibrate() noexcept = 0;
    };

} // namespace Sensors
