#pragma once

#include "Controller.hpp"
#include "ParameterRegistry.hpp"

/**
 * @brief Implémentation concrète d'un contrôleur PID utilisant le registre de paramètres.
 */
class PidController : public Controller {
public:
    explicit PidController(const ParameterRegistry& registry)
        : registry_(registry) {}

    ControlCommands compute(const AttitudeState& state) override {
        // Lecture dynamique des gains à chaque cycle
        float p = registry_.get(ParameterRegistry::ParamId::ROLL_P);
        
        return ControlCommands{
            compute_axis(0.0f, state.roll, p),
            0.0f, // pitch
            0.0f, // yaw
            0.5f  // throttle
        };
    }

private:
    float compute_axis(float target, float current, float p_gain) {
        return (target - current) * p_gain;
    }

    const ParameterRegistry& registry_;
};
