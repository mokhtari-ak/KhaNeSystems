#pragma once
#include "Common/SensorTypes.hpp"

namespace Mocks {

    class GNSSHook : public Sensors::SensorInterface<Sensors::GNSSData> {
    public:
        Sensors::GNSSData update() noexcept override {
            return Sensors::GNSSData{
                .latitude = 48.8584, // Paris (Eiffel Tower) pour simulation
                .longitude = 2.2945,
                .altitude_msl = 100.0f,
                .vel_n = 0.0f,
                .vel_e = 0.0f,
                .vel_d = 0.0f,
                .hdop = 0.8f,
                .fix_type = 3,
                .num_sats = 12,
                .timestamp_us = 0,
                .status_flags = Sensors::IMUData::FLAG_VALID
            };
        }
        bool calibrate() noexcept override { return true; }
    };

    class BaroHook : public Sensors::SensorInterface<Sensors::BaroData> {
    public:
        Sensors::BaroData update() noexcept override {
            return Sensors::BaroData{
                .pressure_pa = 101325.0f,
                .temperature_c = 20.0f,
                .altitude_m = 50.0f,
                .timestamp_us = 0,
                .status_flags = Sensors::IMUData::FLAG_VALID
            };
        }
        bool calibrate() noexcept override { return true; }
    };

    class AirSpeedHook : public Sensors::SensorInterface<Sensors::AirSpeedData> {
    public:
        Sensors::AirSpeedData update() noexcept override {
            return Sensors::AirSpeedData{
                .differential_pressure_pa = 0.0f,
                .indicated_airspeed_ms = 15.0f, // Vitesse de croisière simulée
                .true_airspeed_ms = 15.0f,
                .timestamp_us = 0,
                .status_flags = Sensors::IMUData::FLAG_VALID
            };
        }
        bool calibrate() noexcept override { return true; }
    };

} // namespace Mocks
