// IUartDriver.hpp
#pragma once

#include "stm32f4xx_hal.h"
#include "UartEnumsStructs.hpp"
#include <functional>

using namespace Wrapper;

namespace Hal {

	/// @brief Interface pour le driver UART de bas niveau.
	struct IUartDriver {
		template<typename T>
			uint8_t init();
		
		virtual HAL_StatusTypeDef transmit(int8_t handleIndex, const uint8_t* data, uint16_t size, uint32_t timeout) = 0;
		virtual HAL_StatusTypeDef receive(int8_t handleIndex, uint8_t* data, uint16_t size, uint32_t timeout) = 0;
		virtual HAL_StatusTypeDef transmit_it(int8_t handleIndex, const uint8_t* data, uint16_t size) = 0;
		virtual HAL_StatusTypeDef receive_it(int8_t handleIndex, uint8_t* data, uint16_t size) = 0;
		
		constexpr uint32_t MapWordLength(UartWordLength len);
		constexpr uint32_t MapStopBits(UartStopBits sb);
		constexpr uint32_t MapParity(UartParity p);
		constexpr uint32_t MapMode(UartMode m);
		constexpr uint32_t MapHwControl(UartHwControl hc);
		
		static void attach_callbacks(int8_t handle_index, std::function<void()> tx_cb, std::function<void()> rx_cb);
		static void activate_IRQ(UartPort port);
		
		static void enable_clock();
		static bool is_enabled();
		
		virtual ~IUartDriver() = default;
	};

} // namespace Hal