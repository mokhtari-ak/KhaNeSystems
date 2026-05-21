#pragma once
#include <cstdint>

namespace autopilot {

    /**
     * @brief États principaux du drone.
     */
    enum class flight_state : uint8_t {
        init = 0,
        standby,
        arming,
        armed,
        takeoff,
        flight,
        landing,
        failsafe,
        emergency_stop
    };

    /**
     * @brief Machine à états minimaliste pour valider la structure.
     */
    class state_machine {
    public:
        state_machine() noexcept : m_current_state(flight_state::init) {}

        flight_state get_state() const noexcept { return m_current_state; }

        /**
         * @brief Vérifie si la propulsion est autorisée dans l'état actuel.
         */
        bool is_propulsion_allowed() const noexcept {
            return (m_current_state == flight_state::armed || 
                    m_current_state == flight_state::takeoff || 
                    m_current_state == flight_state::flight || 
                    m_current_state == flight_state::landing ||
                    m_current_state == flight_state::failsafe); // Correction revue : autoriser en failsafe
        }

        void transition_to(flight_state next_state) noexcept {
            // TODO: Ajouter une validation des transitions légales (Point 7 revue)
            m_current_state = next_state;
        }

    private:
        flight_state m_current_state;
    };

} // namespace autopilot
