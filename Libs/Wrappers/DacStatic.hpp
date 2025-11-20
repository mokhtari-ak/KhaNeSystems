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

        /// <summary>
        /// @brief Initialise le périphérique DAC et configure ce canal spécifique.
        /// </summary>
        void init() {
            driver.template init_peripheral<config>();
            driver.template config_channel<config>();
        }
        
        /// <summary>
        /// @brief Écrit une valeur sur la sortie DAC.
        /// </summary>
        /// <param name="value">La valeur à écrire (ex: 0-4095 pour 12 bits)</param>
        void write(uint32_t value) {
            if constexpr (config::CanSet) {
                driver.write(config::Port, config::Channel, config::Align, value);
            }
        }
        
    private:
        Driver driver;
    };

} //namespace Wrapper