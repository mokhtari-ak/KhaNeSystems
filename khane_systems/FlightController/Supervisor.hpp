#pragma once

#include <cstdint>

/**
 * @brief États possibles du système de vol.
 */
enum class SystemState : uint8_t {
    DISARMED = 0,
    CALIBRATING,
    ARMED,
    FAILSAFE
};

/**
 * @brief Gardien de la sécurité (Juge de paix).
 * 
 * Gère les transitions d'états et garantit qu'aucune commande moteur ne soit
 * envoyée dans un état dangereux.
 */
class Supervisor {
public:
    Supervisor() = default;

    /**
     * @brief Tente une transition d'état.
     * @return true si la transition est validée, false sinon.
     */
    bool request_state(SystemState target);

    /**
     * @brief Retourne l'état actuel.
     */
    SystemState get_state() const { return current_state_; }

    /**
     * @brief Vérifie si l'armement des moteurs est autorisé.
     */
    bool can_arm_motors() const { return current_state_ == SystemState::ARMED; }

    /**
     * @brief Point d'entrée pour forcer le mode FAILSAFE.
     * Peut être appelé par n'importe quel module (ou le Valideur) 
     * en cas de détection d'anomalie critique.
     */
    void force_failsafe();

private:
    SystemState current_state_{SystemState::DISARMED};

    /**
     * @brief Valide les règles de transition d'état.
     */
    bool is_transition_valid(SystemState from, SystemState to) const;
};
