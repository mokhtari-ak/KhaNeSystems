#pragma once

#include "PowerMonitor.hpp"
#include "../Drivers/ISensor.hpp"
#include "../Drivers/IBus.hpp"

/**
 * @brief Implémentation concrète pour un capteur de puissance externe (ex: via I2C).
 */
class SmartPowerSensor : public PowerMonitor {
public:
    explicit SmartPowerSensor(IBus& bus) : bus_(bus) {}

    void update() override {
        // Lecture des registres via bus_ (I2C/SPI)
        // Calcul de voltage/current/mah
        // Mise à jour de health_
    }

    PowerData get_data() const override { return data_; }
    BatteryHealth get_health() const override { return health_; }

private:
    IBus& bus_;
    PowerData data_{};
    BatteryHealth health_{BatteryHealth::GOOD};
};
