#pragma once
#include "Autopilot/Inc/StateMachine.hpp"

namespace Control {

    /**
     * @brief Contrôleur de propulsion gérant les moteurs/servos.
     */
    class PropulsionController {
    public:
        PropulsionController(const Autopilot::StateMachine& sm) noexcept : m_stateMachine(sm) {}

        /**
         * @brief Applique une commande de puissance aux moteurs.
         * @param thrust Commande de poussée (0.0f to 1.0f).
         * @return true si la commande a été appliquée, false si interdite par la sécurité.
         */
        bool applyThrust(float thrust) noexcept {
            if (!m_stateMachine.isPropulsionAllowed()) {
                // Règle de sécurité stricte : arrêt immédiat des moteurs si non ARMÉ
                m_currentThrust = 0.0f;
                return false;
            }

            m_currentThrust = thrust;
            // Commande réelle via PWM Wrapper
            return true;
        }

    private:
        const Autopilot::StateMachine& m_stateMachine;
        float m_currentThrust{0.0f};
    };

} // namespace Control
