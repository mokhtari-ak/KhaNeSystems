#pragma once

#include "AttitudeEstimator.hpp"
#include "EigenConfig.hpp"
#include <kalman/ExtendedKalmanFilter.hpp>

/**
 * @brief Implémentation de l'estimateur d'attitude basé sur un EKF.
 * Utilise la bibliothèque Kalman (basée sur Eigen).
 */
class EkfEstimator : public AttitudeEstimator {
public:
    EkfEstimator() {
        // Initialisation de l'état (ex: quat = [1,0,0,0], bias = [0,0,0])
        state_vector_ << 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f;
    }

    void update(const GyroData& gyro, const AccelData& accel) override {
        // 1. Prediction step (utiliser le modèle physique)
        // 2. Correction step (fusionner gyro et accel)
        
        // EKF.predict(...);
        // EKF.update(...);
    }

    AttitudeState get_state() const override {
        // Conversion quaternion -> Euler (roll, pitch, yaw)
        return AttitudeState{ /* ... */ };
    }

private:
    FlightMath::StateVector state_vector_;
    // Kalman::ExtendedKalmanFilter<FlightMath::StateVector> ekf_;
};
