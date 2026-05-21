#include "SensorFusion.hpp"

#include <cmath>

namespace khane::sensor_fusion {

SensorFusion::SensorFusion() noexcept
    : throttler_(&default_throttler_),
      attitude_{1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
      position_{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f} {
}

SensorFusion::SensorFusion(supervisor::CpuThrottler& throttler) noexcept
    : throttler_(&throttler),
      attitude_{1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
      position_{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f} {
}

bool SensorFusion::is_full_model_active() const noexcept {
    return throttler_->get_mode() == supervisor::PerformanceMode::NOMINAL;
}

bool SensorFusion::is_valid_sample(const float* data, std::uint32_t count) noexcept {
    if (data == nullptr) {
        return false;
    }

    for (std::uint32_t index = 0; index < count; ++index) {
        if (!std::isfinite(data[index])) {
            return false;
        }
    }

    return true;
}

void SensorFusion::update_sensors(const float* imu_data, const float* mag_data) noexcept {
    (void)mag_data;

    if (!is_valid_sample(imu_data, 6U)) {
        return;
    }

    // Physical Sanity Checks
    float accel_mag_sq = imu_data[0]*imu_data[0] + imu_data[1]*imu_data[1] + imu_data[2]*imu_data[2];
    if (accel_mag_sq < 64.0f || accel_mag_sq > 144.0f) return;

    attitude_.roll = std::atan2(imu_data[1], imu_data[2]);
    attitude_.pitch = std::atan2(-imu_data[0], std::sqrt((imu_data[1] * imu_data[1]) + (imu_data[2] * imu_data[2])));

    if (is_full_model_active()) {
        position_.vx = imu_data[3];
        position_.vy = imu_data[4];
        position_.vz = imu_data[5];
    } else {
        position_.vx = 0.0f;
        position_.vy = 0.0f;
        position_.vz = 0.0f;
    }
}

Attitude SensorFusion::get_attitude() const noexcept {
    return attitude_;
}

Position SensorFusion::get_position() const noexcept {
    return position_;
}

} // namespace khane::sensor_fusion
