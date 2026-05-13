#pragma once

#include <cmath>
#include <cstdint>

#include "ImuDriverInputAdapter.hpp"
#include "Modules/KalmanFilterLibrary/include/kalman/ExtendedKalmanFilter.hpp"
#include "Modules/KalmanFilterLibrary/include/kalman/LinearizedMeasurementModel.hpp"
#include "Modules/KalmanFilterLibrary/include/kalman/LinearizedSystemModel.hpp"

namespace Drivers::SensorFusion {

struct EkfFusionConfig {
    float accel_scale = 1.0f;
    float gyro_scale = 1.0f;
    float process_noise_angle = 1e-3f;
    float process_noise_bias = 1e-5f;
    float measurement_noise_accel = 5e-2f;
};

struct EkfFusionState {
    std::uint64_t timestamp_us = 0;
    float roll = 0.0f;
    float pitch = 0.0f;
    float yaw = 0.0f;
    bool valid = false;
};

class EkfFusionNode {
public:
    EkfFusionNode(ImuDriverInputAdapter& imu_input, EkfFusionConfig config = {}) noexcept
        : imu_input_(imu_input), config_(config) {}

    void init() noexcept {
        initialized_ = true;
        state_ = {};
        previous_timestamp_us_ = 0;
        state_vector_.setZero();
        ekf_.init(state_vector_);
        configureCovariances();
    }

    const EkfFusionState& step(std::uint64_t timestamp_us) noexcept {
        if (!initialized_) {
            state_.valid = false;
            return state_;
        }

        const ImuFusionInput imu = imu_input_.pull(timestamp_us);
        state_.timestamp_us = timestamp_us;
        state_.valid = imu.valid;

        if (!imu.valid) {
            return state_;
        }

        const float dt = computeDtSec(timestamp_us);
        Control control;
        control.setZero();
        control(0) = imu.gyro_x * config_.gyro_scale;
        control(1) = imu.gyro_y * config_.gyro_scale;
        control(2) = imu.gyro_z * config_.gyro_scale;
        control(3) = dt;
        const State& predicted = ekf_.predict(system_model_, control);

        const float ax = imu.accel_x * config_.accel_scale;
        const float ay = imu.accel_y * config_.accel_scale;
        const float az = imu.accel_z * config_.accel_scale;
        const float norm = std::sqrt((ax * ax) + (ay * ay) + (az * az));
        if (norm > 1e-6f) {
            Measurement z;
            z(0) = ax / norm;
            z(1) = ay / norm;
            z(2) = az / norm;
            const State& updated = ekf_.update(measurement_model_, z);
            state_.roll = updated(0);
            state_.pitch = updated(1);
            state_.yaw = updated(2);
        } else {
            state_.roll = predicted(0);
            state_.pitch = predicted(1);
            state_.yaw = predicted(2);
        }
        return state_;
    }

    const EkfFusionState& state() const noexcept { return state_; }

private:
    using Scalar = float;
    using State = Kalman::Vector<Scalar, 6>;       // [roll pitch yaw bgx bgy bgz]
    using Control = Kalman::Vector<Scalar, 4>;     // [wx wy wz dt]
    using Measurement = Kalman::Vector<Scalar, 3>; // [ax ay az] normalized

    class OrientationSystemModel final : public Kalman::LinearizedSystemModel<State, Control> {
    public:
        State f(const State& x, const Control& u) const override {
            State out = x;
            const Scalar dt = u(3);
            out(0) = x(0) + (u(0) - x(3)) * dt;
            out(1) = x(1) + (u(1) - x(4)) * dt;
            out(2) = x(2) + (u(2) - x(5)) * dt;
            out(3) = x(3);
            out(4) = x(4);
            out(5) = x(5);
            return out;
        }

    protected:
        void updateJacobians(const State&, const Control& u) override {
            this->F.setIdentity();
            const Scalar dt = u(3);
            this->F(0, 3) = -dt;
            this->F(1, 4) = -dt;
            this->F(2, 5) = -dt;
            this->W.setIdentity();
        }
    };

    class GravityMeasurementModel final : public Kalman::LinearizedMeasurementModel<State, Measurement> {
    public:
        Measurement h(const State& x) const override {
            const Scalar roll = x(0);
            const Scalar pitch = x(1);
            Measurement z;
            z(0) = -std::sin(pitch);
            z(1) = std::sin(roll) * std::cos(pitch);
            z(2) = std::cos(roll) * std::cos(pitch);
            return z;
        }

    protected:
        void updateJacobians(const State& x) override {
            const Scalar roll = x(0);
            const Scalar pitch = x(1);
            this->H.setZero();
            this->H(0, 1) = -std::cos(pitch);
            this->H(1, 0) = std::cos(roll) * std::cos(pitch);
            this->H(1, 1) = -std::sin(roll) * std::sin(pitch);
            this->H(2, 0) = -std::sin(roll) * std::cos(pitch);
            this->H(2, 1) = -std::cos(roll) * std::sin(pitch);
            this->V.setIdentity();
        }
    };

    float computeDtSec(std::uint64_t timestamp_us) noexcept {
        if (previous_timestamp_us_ == 0 || timestamp_us <= previous_timestamp_us_) {
            previous_timestamp_us_ = timestamp_us;
            return 0.001f;
        }
        const auto delta = timestamp_us - previous_timestamp_us_;
        previous_timestamp_us_ = timestamp_us;
        return static_cast<float>(delta) / 1000000.0f;
    }

    void configureCovariances() noexcept {
        Kalman::Covariance<State> q;
        q.setZero();
        q(0, 0) = config_.process_noise_angle;
        q(1, 1) = config_.process_noise_angle;
        q(2, 2) = config_.process_noise_angle;
        q(3, 3) = config_.process_noise_bias;
        q(4, 4) = config_.process_noise_bias;
        q(5, 5) = config_.process_noise_bias;
        (void)system_model_.setCovariance(q);

        Kalman::Covariance<Measurement> r;
        r.setZero();
        r(0, 0) = config_.measurement_noise_accel;
        r(1, 1) = config_.measurement_noise_accel;
        r(2, 2) = config_.measurement_noise_accel;
        (void)measurement_model_.setCovariance(r);
    }

    ImuDriverInputAdapter& imu_input_;
    EkfFusionConfig config_{};
    EkfFusionState state_{};
    bool initialized_ = false;
    std::uint64_t previous_timestamp_us_ = 0;
    State state_vector_{};
    Kalman::ExtendedKalmanFilter<State> ekf_{};
    OrientationSystemModel system_model_{};
    GravityMeasurementModel measurement_model_{};
};

} // namespace Drivers::SensorFusion
