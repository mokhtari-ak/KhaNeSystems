#pragma once

#include "../Drivers/IMU/MPU6050.hpp" // Exemple de type de donnée
#include <array>

/**
 * @brief État estimé du drone (Attitude).
 */
struct AttitudeState {
    float roll, pitch, yaw;
};

/**
 * @brief Interface pour l'estimation d'attitude (EKF/Complémentaire).
 */
class AttitudeEstimator {
public:
    virtual ~AttitudeEstimator() = default;

    // Mise à jour de l'estimation avec les données brutes des capteurs
    virtual void update(const GyroData& gyro, const AccelData& accel) = 0;
    
    // Récupération de l'état actuel
    virtual AttitudeState get_state() const = 0;
};
