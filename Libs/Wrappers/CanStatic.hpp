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
        
        /*
        * @Brief Initialise le CAN (Timing, Mode) et configure le filtre statique.
        **/ 
        void init() {
            driver.template init_peripheral<config>();
            driver.template config_filter<config>();
            
            // Démarrer le périphérique CAN
            HAL_CAN_Start(&Driver::canHandles[config::Port]);
        }
        /**
        * @Brief Envoie un message CAN (mode blocant). Retourne vrai si l'envoi a réussi.
        * @Note : Si config::CanSend est faux, cette méthode ne fera rien et retournera toujours faux.
        */
        bool send(const CanMessage& message) {
            if constexpr (config::CanSend) {
                return driver.transmit(config::Port, message);
            }
            return false;
        }
        
        /**
        * @Brief Attache un callback à l'interruption de réception (pour le message filtré).
        */
        void attach_rx_callback(std::function<void(const CanMessage&)> cb) {
            if constexpr (config::CanReceive && config::UseInterrupt) {
                driver.attach_rx_interrupt(config::Port, config::Fifo, cb);
            }
        }
        
        /**
        * @Brief Lit l'état du bus CAN.
        * @Return L'état du bus CAN.
        */
        CanBusState get_bus_state() {
            return driver.get_bus_state(config::Port);
        }

    private:
        Driver driver;
    };

} //namespace Wrapper
