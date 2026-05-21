#pragma once

#include <algorithm>

#include "autopilot/state_machine.hpp"

namespace control {

    class propulsion_controller {
    public:
        // Utilisation d'un pointeur pour éviter le dangling reference si l'objet est réassigné
        // Bien que dans ce système, les objets soient statiques.
        propulsion_controller(const autopilot::state_machine& sm) noexcept : m_state_machine(sm) {}

        /**
         * @brief Applique une commande de puissance aux moteurs.
         * @param thrust Commande de poussée (0.0f to 1.0f).
         * @return true si la commande a été appliquée, false si interdite par la sécurité.
         */
        bool apply_thrust(float thrust, float battery_voltage) noexcept {
            if (!m_state_machine.is_propulsion_allowed()) {
                m_current_thrust = 0.0f;
                return false;
            }

            // Voltage Sag Protection: Limit thrust if voltage is low
            static constexpr float VOLTAGE_THRESHOLD = 10.0f;
            float limit = (battery_voltage < VOLTAGE_THRESHOLD) ? 0.5f : 1.0f;
            m_current_thrust = std::clamp(thrust, 0.0f, limit);
            
            return true;
        }

        float get_current_thrust() const noexcept { return m_current_thrust; }

    private:
        const autopilot::state_machine& m_state_machine;
        float m_current_thrust{0.0f};
    };

} // namespace control
