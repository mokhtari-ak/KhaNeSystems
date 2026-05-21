#pragma once

#include "MotorMixer.hpp"

/**
 * @brief Implémentation concrète du mixage moteur pour protocoles DShot.
 */
class DshotMixer : public MotorMixer {
public:
    void apply(const ControlCommands& commands) override {
        // Algorithme de mixage quadcoptère X standard
        // M1 = Throttle + Roll + Pitch - Yaw
        // M2 = Throttle - Roll + Pitch + Yaw
        // M3 = Throttle - Roll - Pitch - Yaw
        // M4 = Throttle + Roll - Pitch + Yaw
        
        // Envoi des commandes via le driver matériel DShot
    }

    void apply_safe_state() override {
        // Envoi de la commande 0 (arrêt moteur) à tous les moteurs
    }
};
