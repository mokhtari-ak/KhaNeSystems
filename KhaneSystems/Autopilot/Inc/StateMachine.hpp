#pragma once
#include "AutopilotTypes.hpp"
#include "RtosAbstract/Inc/RtosControl.hpp"

namespace Autopilot {

    /**
     * @brief Classe gérant la machine d'état principale de l'application.
     */
    class StateMachine {
    public:
        StateMachine(VehicleType type) noexcept : m_vehicleType(type) {}

        /**
         * @brief Met à jour la machine d'état (appelée périodiquement).
         */
        void update() noexcept;

        /**
         * @brief Tente une transition vers un nouvel état.
         */
        bool requestTransition(SystemState newState, TransitionReason reason, TransitionSource source) noexcept;

        /**
         * @brief Accesseurs.
         */
        SystemState getCurrentState() const noexcept { return m_currentState; }
        VehicleType getVehicleType() const noexcept { return m_vehicleType; }

        /**
         * @brief Règle de sécurité critique : vérifie si la propulsion est autorisée.
         */
        bool isPropulsionAllowed() const noexcept {
            return m_currentState == SystemState::Armed;
        }

    private:
        SystemState m_currentState{SystemState::Disarmed};
        VehicleType m_vehicleType;

        void logTransition(SystemState oldState, SystemState newState, TransitionReason reason, TransitionSource source) noexcept;
        void handleFailsafeEntry() noexcept;
    };

} // namespace Autopilot
