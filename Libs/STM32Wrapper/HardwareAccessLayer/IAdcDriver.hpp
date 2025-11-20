#pragma once

#include "AdcEnumsStructs.hpp"
#include "AdcConfigPolicy.hpp"
#include "stm32f4xx_hal.h" // Inclure la base HAL

using namespace WrapperBase;

namespace Hal {

	struct IAdcDriver {
        
		/// <summary>
		/// @brief Initialise le périphérique ADC principal (horloge, résolution, etc.)
		/// </summary>
		template <AdcConfigPolicy config>
			void init_peripheral();
        
		/// <summary>
		/// @brief Configure un canal spécifique sur le périphérique ADC.
		/// </summary>
		template <AdcConfigPolicy config>
			void config_channel();

		/// <summary>
		/// @brief Lance une conversion et lit la valeur (blocage).
		/// </summary>
		virtual uint32_t read(AdcPort port) = 0;
        
		// --- Fonctions d'aide statiques pour le mappage ---
        
		static ADC_TypeDef* MapPort(AdcPort port);
		static uint32_t MapChannel(AdcChannel channel);
		static uint32_t MapResolution(AdcResolution res);
		static uint32_t MapSampleTime(AdcSampleTime time);
		static void enable_clock(AdcPort port);
		static bool is_clock_enabled(AdcPort port);

		virtual ~IAdcDriver() = default;
	};

} // namespace Hal