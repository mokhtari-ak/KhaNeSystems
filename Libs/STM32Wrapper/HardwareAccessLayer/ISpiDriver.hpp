// ISpiDriver.hpp
#pragma once

#include "stm32f4xx_hal.h"
#include "SpiEnumsStructs.hpp"
#include "SpiConfigPolicy.hpp"
#include <functional>

using namespace Wrapper;
using namespace WrapperBase;

namespace Hal {

	/// @brief Interface pour le driver SPI de bas niveau.
	struct ISpiDriver {
		template<SpiConfigPolicy T>
			int8_t init();

		// Fonctions bloquantes
		virtual HAL_StatusTypeDef transmit(int8_t handleIndex, const uint8_t* data, uint16_t size, uint32_t timeout) = 0;
		virtual HAL_StatusTypeDef receive(int8_t handleIndex, uint8_t* data, uint16_t size, uint32_t timeout) = 0;
		virtual HAL_StatusTypeDef transmit_receive(int8_t handleIndex, const uint8_t* txData, uint8_t* rxData, uint16_t size, uint32_t timeout) = 0;

		// Fonctions non-bloquantes (Interrupt)
		virtual HAL_StatusTypeDef transmit_it(int8_t handleIndex, const uint8_t* data, uint16_t size) = 0;
		virtual HAL_StatusTypeDef receive_it(int8_t handleIndex, uint8_t* data, uint16_t size) = 0;
		virtual HAL_StatusTypeDef transmit_receive_it(int8_t handleIndex, const uint8_t* txData, uint8_t* rxData, uint16_t size) = 0;

		// Fonctions statiques de mapping et de gestion
		static constexpr SPI_TypeDef* MapPort(SpiPort port);
		static constexpr uint32_t MapMode(SpiMode mode);
		static constexpr uint32_t MapDirection(SpiDirection dir);
		static constexpr uint32_t MapDataSize(SpiDataSize size);
		static constexpr uint32_t MapClockPolarity(SpiClockPolarity cpol);
		static constexpr uint32_t MapClockPhase(SpiClockPhase cpha);
		static constexpr uint32_t MapNssMode(SpiNssMode nss);
		static constexpr uint32_t MapPrescaler(SpiBaudRatePrescaler psc);
		static constexpr uint32_t MapFirstBit(SpiFirstBit fb);

		static void attach_callbacks(int8_t handle_index,
			std::function<void()> tx_cb,
			std::function<void()> rx_cb,
			std::function<void()> txrx_cb,
			std::function<void()> error_cb);

		static void activate_IRQ(SpiPort port);
		static void enable_clock(SpiPort port);
		static bool is_enabled(SpiPort port);

		virtual ~ISpiDriver() = default;
	};

} // namespace Hal