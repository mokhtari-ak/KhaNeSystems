#pragma once
#include "Propulsion/Inc/PropulsionController.hpp"

namespace Control {

    /**
     * @brief Contrôleur de vol abstrait.
     */
    class FlightController {
    public:
        FlightController(PropulsionController& pc) noexcept : m_propulsion(pc) {}

        virtual void update() noexcept = 0;

    protected:
        PropulsionController& m_propulsion;
    };

} // namespace Control
