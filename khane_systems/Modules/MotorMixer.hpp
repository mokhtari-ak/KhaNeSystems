#pragma once

#include "Controller.hpp"

/**
 * @brief Interface pour le mixage moteur.
 * Transforme les commandes de vol en signaux pour chaque moteur.
 */
class MotorMixer {
public:
    virtual ~MotorMixer() = default;

    // Applique les commandes aux moteurs physiques
    virtual void apply(const ControlCommands& commands) = 0;

    // Force les moteurs à l'arrêt ou état sécurisé
    virtual void apply_safe_state() = 0;
};
