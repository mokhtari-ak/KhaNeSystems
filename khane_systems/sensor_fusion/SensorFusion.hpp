#pragma once

#include "ISensorFusion.hpp"
#include "../supervisor/CpuThrottler.hpp"

namespace khane::sensor_fusion {

class SensorFusion : public ISensorFusion {
public:
    SensorFusion() noexcept;
    explicit SensorFusion(supervisor::CpuThrottler& throttler) noexcept;

    void update_sensors(const float* imu_data, const float* mag_data) noexcept override;
    Attitude get_attitude() const noexcept override;
    Position get_position() const noexcept override;

private:
    supervisor::CpuThrottler default_throttler_{};
    supervisor::CpuThrottler* throttler_;
    Attitude attitude_;
    Position position_;
    bool is_full_model_active() const noexcept;
    static bool is_valid_sample(const float* data, std::uint32_t count) noexcept;
};

} // namespace khane::sensor_fusion
