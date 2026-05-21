#include "StateMachine.hpp"

namespace Autopilot {

    void StateMachine::update() noexcept {
        // Logique périodique de surveillance de l'état
        if (m_currentState == SystemState::Failsafe) {
            handleFailsafeEntry();
        }
    }

    bool StateMachine::requestTransition(SystemState newState, TransitionReason reason, TransitionSource source) noexcept {
        if (newState == m_currentState) return true;

        // Validation des transitions critiques
        bool allowed = false;

        switch (m_currentState) {
            case SystemState::Disarmed:
                // Pour armer, il faut une commande utilisateur ou une mission auto
                if (newState == SystemState::Armed) {
                    // Ici on ajouterait la Checklist de sécurité
                    allowed = true; 
                }
                break;

            case SystemState::Armed:
                // On peut désarmer ou tomber en Failsafe
                if (newState == SystemState::Disarmed || newState == SystemState::Failsafe) {
                    allowed = true;
                }
                break;

            case SystemState::Failsafe:
                // Sortir du Failsafe nécessite souvent un désarmement manuel
                if (newState == SystemState::Disarmed && source == TransitionSource::Operator) {
                    allowed = true;
                }
                break;
        }

        if (allowed) {
            SystemState oldState = m_currentState;
            m_currentState = newState;
            logTransition(oldState, newState, reason, source);
            return true;
        }

        return false;
    }

    void StateMachine::logTransition(SystemState oldState, SystemState newState, TransitionReason reason, TransitionSource source) noexcept {
        // Ici, on utiliserait le module de Logging/Télémétrie
        // Pour l'instant, c'est un point d'ancrage pour l'implémentation future
        (void)oldState;
        (void)newState;
        (void)reason;
        (void)source;
    }

    void StateMachine::handleFailsafeEntry() noexcept {
        // Action immédiate de sécurité : 
        // Si Quadcopter -> Descente lente
        // Si FixedWing -> Plané circulaire
        if (m_vehicleType == VehicleType::Quadcopter) {
            // Commande moteur de sécurité
        } else {
            // Commande gouvernes de sécurité
        }
    }

} // namespace Autopilot
