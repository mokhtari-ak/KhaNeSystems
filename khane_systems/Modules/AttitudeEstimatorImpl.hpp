#pragma once

#include "AttitudeEstimator.hpp"
#include "EkfEstimator.hpp"

/**
 * @brief Implémentation concrète de l'estimateur d'attitude.
 * Encapsule l'EKF et le présente au FlightController.
 */
class AttitudeEstimatorImpl : public AttitudeEstimator {
public:
    void update(const GyroData& gyro, const AccelData& accel) override {
        // Le module EkfEstimator gère la logique complexe du filtre de Kalman
        ekf_.update(gyro, accel);
    }
    
    AttitudeState get_state() const override {
        // Conversion de l'état EKF vers AttitudeState (Roll, Pitch, Yaw)
        return ekf_.get_state();
    }

private:
    EkfEstimator ekf_;
};
