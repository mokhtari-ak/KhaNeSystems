#include "Supervisor.hpp"

bool Supervisor::request_state(SystemState target) {
    if (is_transition_valid(current_state_, target)) {
        current_state_ = target;
        return true;
    }
    return false;
}

void Supervisor::force_failsafe() {
    // FAILSAFE est une priorité absolue.
    current_state_ = SystemState::FAILSAFE;
}

bool Supervisor::is_transition_valid(SystemState from, SystemState to) const {
    // Si déjà en FAILSAFE, impossible d'en sortir sans redémarrage ou action spéciale
    if (from == SystemState::FAILSAFE) {
        return false;
    }

    switch (from) {
        case SystemState::DISARMED:
            return (to == SystemState::CALIBRATING || to == SystemState::ARMED);
            
        case SystemState::CALIBRATING:
            return (to == SystemState::DISARMED);
            
        case SystemState::ARMED:
            return (to == SystemState::DISARMED || to == SystemState::FAILSAFE);
            
        default:
            return false;
    }
}
