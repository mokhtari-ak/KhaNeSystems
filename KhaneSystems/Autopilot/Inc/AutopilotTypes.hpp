#pragma once
#include <cstdint>

namespace Autopilot {

    /**
     * @brief États principaux de la machine d'état de l'Autopilot.
     */
    enum class SystemState : uint8_t {
        Disarmed,   // Moteurs coupés, système au repos
        Armed,      // Moteurs actifs, prêt au vol ou en vol
        Failsafe    // Mode sécurité critique, action corrective en cours
    };

    /**
     * @brief Type de véhicule supporté.
     */
    enum class VehicleType : uint8_t {
        Quadcopter,
        FixedWing
    };

    /**
     * @brief Raison d'une transition d'état.
     */
    enum class TransitionReason : uint8_t {
        UserCommand,
        AutoMission,
        RC_Lost,
        BatteryCritical,
        SensorFailure,
        EKF_Divergence,
        WatchdogTimeout
    };

    /**
     * @brief Origine de la demande de transition.
     */
    enum class TransitionSource : uint8_t {
        Operator,
        Autopilot
    };

} // namespace Autopilot
