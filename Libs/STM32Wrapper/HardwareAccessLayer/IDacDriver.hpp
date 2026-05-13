#pragma once

#include "DacEnumsStructs.hpp"
#include "DacConfigPolicy.hpp"
#include "stm32f4xx_hal.h" // Inclure la base HAL

using namespace WrapperBase;

namespace Hal {

	struct IDacDriver {
        
		/// <summary>
		/// @brief Initialise le périphérique DAC principal (horloge).
		/// </summary>
		template <DacConfigPolicy config>
			void init_peripheral();
        
		/// <summary>
		/// @brief Configure un canal spécifique sur le périphérique DAC.
		/// </summary>
		template <DacConfigPolicy config>
			void config_channel();

		/// <summary>
		/// @brief Écrit une valeur sur le canal DAC.
		/// </summary>
		virtual void write(DacPort port, DacChannel channel, DacDataAlign align, uint32_t value) = 0;
        
		/// <summary>
		/// @brief Démarre la sortie DAC.
		/// </summary>
		virtual void start(DacPort port, DacChannel channel) = 0;

		/// <summary>
		/// @brief Arrête la sortie DAC.
		/// </summary>
		virtual void stop(DacPort port, DacChannel channel) = 0;

		// --- Fonctions d'aide statiques pour le mappage ---
        
		static DAC_TypeDef* MapPort(DacPort port);
		static uint32_t MapChannel(DacChannel channel);
		static uint32_t MapAlign(DacDataAlign align);
		static uint32_t MapOutputBuffer(bool enabled);
        
		static void enable_clock(DacPort port);
		static bool is_clock_enabled(DacPort port);

		virtual ~IDacDriver() = default;
	};

} // namespace Hal