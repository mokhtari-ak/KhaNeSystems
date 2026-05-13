#pragma once

#include "EkfFusionNode.hpp"

namespace Drivers::SensorFusion {

enum class EkfProfile {
    Sim,
    Bench,
    Flight
};

struct EkfTuning {
    static constexpr EkfFusionConfig forProfile(EkfProfile profile) noexcept {
        switch (profile) {
        case EkfProfile::Sim:
            return EkfFusionConfig{
                .accel_scale = 1.0f,
                .gyro_scale = 1.0f,
                .process_noise_angle = 5e-4f,
                .process_noise_bias = 1e-6f,
                .measurement_noise_accel = 2e-2f,
            };
        case EkfProfile::Bench:
            return EkfFusionConfig{
                .accel_scale = 1.0f,
                .gyro_scale = 1.0f,
                .process_noise_angle = 1e-3f,
                .process_noise_bias = 5e-6f,
                .measurement_noise_accel = 5e-2f,
            };
        case EkfProfile::Flight:
            return EkfFusionConfig{
                .accel_scale = 1.0f,
                .gyro_scale = 1.0f,
                .process_noise_angle = 2e-3f,
                .process_noise_bias = 1e-5f,
                .measurement_noise_accel = 8e-2f,
            };
        }

        return EkfFusionConfig{};
    }
};

} // namespace Drivers::SensorFusion
