#pragma once

#include "../Modules/AttitudeEstimator.hpp"
#include "../FlightController/FlightController.hpp"

/**
 * @brief Commandes de navigation (sorties du contrôleur de position).
 */
struct NavCommands {
    float roll_setpoint;  // Consigne d'angle en radians
    float pitch_setpoint; // Consigne d'angle en radians
    float yaw_rate;       // Consigne de vitesse de rotation
};

/**
 * @brief Interface pour le contrôleur de navigation (Boucle de position).
 */
class NavigationController {
public:
    virtual ~NavigationController() = default;

    // Calcule les consignes d'attitude à partir de la position actuelle et cible
    virtual NavCommands compute(const AttitudeState& current_state, const struct Position& target_pos) = 0;
};
