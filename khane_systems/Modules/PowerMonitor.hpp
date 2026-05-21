#pragma once

#include "../Drivers/ISensor.hpp"
#include "IStorage.hpp" // Pour le logging si besoin
#include <cstdint>

/**
 * @brief État de santé de la batterie.
 */
enum class BatteryHealth {
    GOOD,
    LOW_WARNING,
    CRITICAL
};

/**
 * @brief Données électriques mesurées.
 */
struct PowerData {
    float voltage;       // Volts
    float current;       // Amperes
    float consumed_mah;  // mAh
};

/**
 * @brief Interface pour le monitoring de puissance.
 */
class PowerMonitor {
public:
    virtual ~PowerMonitor() = default;

    // Mise à jour de l'état du capteur (doit être appelé périodiquement)
    virtual void update() = 0;

    // Récupération des données brutes
    virtual PowerData get_data() const = 0;

    // Récupération de l'état de santé pour le Supervisor
    virtual BatteryHealth get_health() const = 0;
};
