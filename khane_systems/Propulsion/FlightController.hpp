#pragma once

#include <algorithm>

namespace control {

    struct flight_control_command {
        float throttle{0.0f};
        float pitch{0.0f};
        float roll{0.0f};
        float yaw{0.0f};
    };

    struct QuadcopterDynamics {
        static constexpr bool IS_FIXED_WING = false;

        void apply_control(float throttle, float pitch, float roll, float yaw) noexcept {
            last_command_ = {std::clamp(throttle, 0.0f, 1.0f), pitch, roll, yaw};
        }

        flight_control_command get_last_command() const noexcept {
            return last_command_;
        }

    private:
        flight_control_command last_command_{};
    };

    struct FixedWingDynamics {
        static constexpr bool IS_FIXED_WING = true;

        void apply_control(float throttle, float pitch, float roll, float yaw) noexcept {
            last_command_ = {std::clamp(throttle, 0.0f, 1.0f), pitch, roll, yaw};
        }

        flight_control_command get_last_command() const noexcept {
            return last_command_;
        }

    private:
        flight_control_command last_command_{};
    };

    template <typename T>
    class FlightController {
    public:
        void update(float throttle, float pitch, float roll, float yaw) noexcept {
            dynamics_.apply_control(throttle, pitch, roll, yaw);
        }

        const T& get_dynamics() const noexcept {
            return dynamics_;
        }

    private:
        T dynamics_;
    };

} // namespace control
