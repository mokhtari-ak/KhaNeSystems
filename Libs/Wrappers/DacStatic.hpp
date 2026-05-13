#pragma once

#include "DacEnumsStructs.hpp"
#include "IDacDriver.hpp"
#include "DacDriver.hpp"
#include "DacConfigPolicy.hpp"

using namespace Hal;
using namespace WrapperBase;

namespace Wrapper {

    template<DacConfigPolicy config, typename Driver = HalDacDriver>
    class DacStatic {
    public:
        DacStatic() = default;

        /**
        * @Brief Initialise le périphérique DAC et configure ce canal spécifique.*/
        void init() {
            driver.template init_peripheral<config>();
            driver.template config_channel<config>();
        }
        
        /**
        * @Brief écrit une valeur sur la sortie DAC.
        * @Note : Si config::CanSet est faux, cette méthode ne fera rien.
        * @Param value La valeur à écrire (ex: 0-4095 pour 12 bits)
        */
        void write(uint32_t value) {
            if constexpr (config::CanSet) {
                driver.write(config::Port, config::Channel, config::Align, value);
            }
        }
        
    private:
        Driver driver;
    };

} //namespace Wrapper