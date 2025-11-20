#pragma once

#include "CanEnumsStructs.hpp"
#include "ICanDriver.hpp"
#include "CanDriver.hpp" // (Doit être inclus)
#include "CanConfigPolicy.hpp"

using namespace Hal;
using namespace WrapperBase;

namespace Wrapper {

    template<CanConfigPolicy config, typename Driver = HalCanDriver>
    class CanStatic {
    public:
        CanStatic() = default;

        /// @brief Initialise le CAN (Timing, Mode) et configure le filtre statique.
        void init() {
            driver.template init_peripheral<config>();
            driver.template config_filter<config>();
            
            // Démarrer le périphérique CAN
            HAL_CAN_Start(&Driver::canHandles[config::Port]);
        }
        
        /// @brief Envoie un message CAN.
        /// @param message Le message à envoyer (ID, Data, Longueur).
        bool send(const CanMessage& message) {
            if constexpr (config::CanSend) {
                return driver.transmit(config::Port, message);
            }
            return false;
        }
        
        /// @brief Attache un callback à l'interruption de réception (pour le message filtré).
        void attach_rx_callback(std::function<void(const CanMessage&)> cb) {
            if constexpr (config::CanReceive && config::UseInterrupt) {
                driver.attach_rx_interrupt(config::Port, config::FilterConfig.Fifo, cb);
            }
        }
        
        // On peut ajouter une méthode pour lire l'état du bus, le nombre d'erreurs, etc.

    private:
        Driver driver;
    };

} //namespace Wrapper