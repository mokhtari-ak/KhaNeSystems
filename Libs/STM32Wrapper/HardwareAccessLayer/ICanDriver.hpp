#pragma once

#include "CAN/CanEnumsStructs.hpp"
#include "CAN/CanConfigPolicy.hpp"
#include "stm32f4xx_hal.h" 
#include <functional>

using namespace WrapperBase;

namespace Hal {

	struct ICanDriver {
        
		/// @brief Initialise le périphérique CAN et le Bit Timing.
		template <CanConfigPolicy config>
			void init_peripheral();
        
		/// @brief Configure les filtres du CAN.
		template <CanConfigPolicy config>
			void config_filter();

		/// @brief Envoie un message sur le bus (non-blocant).
		virtual bool transmit(CanPort port, const CanMessage& message) = 0;
        
		/// @brief Réception d'un message via la FIFO (pollé/blocant).
		virtual bool receive_polling(CanPort port, CanRxFifo fifo, CanMessage& message) = 0;
        
		/// @brief Attache un callback à la réception d'un message par interruption.
		virtual void attach_rx_interrupt(CanPort port, CanRxFifo fifo, std::function<void(const CanMessage&)> cb) = 0;

		// --- Fonctions d'aide statiques ---
		// Vous aurez besoin de fonctions statiques similaires à celles du GPIO/ADC
		// pour mapper les enums vers les constantes HAL (`CAN_MODE_NORMAL`, `CAN_TX_TYPE_STDID`, etc.)
        
		virtual ~ICanDriver() = default;
	};

} // namespace Hal