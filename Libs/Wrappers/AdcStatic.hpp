#pragma once

#include "AdcEnumsStructs.hpp"
#include "IAdcDriver.hpp"
#include "AdcDriver.hpp"
#include "AdcConfigPolicy.hpp"

using namespace Hal;
using namespace WrapperBase;

namespace Wrapper {

	template<AdcConfigPolicy config, typename Driver = HalAdcDriver>
		class AdcStatic {
		public:
			AdcStatic() = default;

			/**
			 * @Brief Initialise le périphérique ADC et configure ce canal spécifique.
			 */
			void init() {
				driver.template init_peripheral<config>();
				driver.template config_channel<config>();
			}
			/**
			 * @Brief Lit la valeur du canal ADC configuré (mode blocant).
			 * @Return La valeur convertie (0-4095 pour 12 bits)
			 */

			 uint32_t read() {
				// if constexpr (config::CanRead) // Toujours vrai pour l'ADC
				return driver.read(config::Port);
			}
        
			// On pourrait ajouter attach_interrupt() ici si on gérait les IT
        
		private:
			Driver driver;
		};

} //namespace Wrapper