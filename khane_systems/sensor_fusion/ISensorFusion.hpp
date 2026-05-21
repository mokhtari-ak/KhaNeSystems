#pragma once

#include <cstdint>

namespace khane::sensor_fusion {

struct Attitude {
    float qw, qx, qy, qz;
    float roll, pitch, yaw;
};

struct Position {
    float x, y, z;
    float vx, vy, vz;
};

class ISensorFusion {
public:
    virtual ~ISensorFusion() = default;
    virtual void update_sensors(const float* imu_data, const float* mag_data) noexcept = 0;
    virtual Attitude get_attitude() const noexcept = 0;
    virtual Position get_position() const noexcept = 0;
};

} // namespace khane::sensor_fusion
