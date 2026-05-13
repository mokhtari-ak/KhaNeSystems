#pragma once

#include "AdcEnumsStructs.hpp"
#include "AdcConfigPolicy.hpp"
#include "stm32f4xx_hal.h" // Inclure la base HAL

using namespace WrapperBase;

namespace Hal {

	struct IAdcDriver {
        
		/** 
		 * --- Méthodes d'interface ---
		 * @Brief Initialise le périphérique ADC principal (horloge, résolution, etc.)
		*/
		template <AdcConfigPolicy config>
		virtual	void init_peripheral() = 0;
        
		/** 
		 * @Brief Configure un canal spécifique sur le périphérique ADC.
		 */
		template <AdcConfigPolicy config>
			virtual void config_channel() = 0;

		/**
		* @Brief Lance une conversion et lit la valeur (blocage).
		* @Note Cette fonction lance une conversion sur le canal spécifié et lit la valeur résultante.
		*/
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