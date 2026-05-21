#pragma once

#include <cstdint>
#include <algorithm>
#include <string_view>

/**
 * @brief Registre central pour les paramètres de vol ajustables.
 */
struct Parameter {
    float value;
    float min;
    float max;
    const char* name;
};

class ParameterRegistry {
public:
    static constexpr size_t MAX_PARAMS = 16;

    struct ParamId {
        static constexpr uint16_t ROLL_P = 0;
        static constexpr uint16_t ROLL_I = 1;
        static constexpr uint16_t ROLL_D = 2;
        static constexpr uint16_t PITCH_P = 3;
        static constexpr uint16_t PITCH_I = 4;
        static constexpr uint16_t PITCH_D = 5;
    };

    ParameterRegistry() {
        params_[ParamId::ROLL_P] = {0.1f, 0.0f, 1.0f, "ROLL_P"};
        // ... initialisation des autres paramètres
    }

    void update(uint16_t id, float value) {
        if (id < MAX_PARAMS) {
            params_[id].value = std::clamp(value, params_[id].min, params_[id].max);
        }
    }

    float get(uint16_t id) const {
        return (id < MAX_PARAMS) ? params_[id].value : 0.0f;
    }

private:
    std::array<Parameter, MAX_PARAMS> params_{};
};
