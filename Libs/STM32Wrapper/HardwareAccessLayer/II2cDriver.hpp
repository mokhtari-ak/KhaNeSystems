#pragma once
#include "stm32f4xx_hal.h"
#include "I2cEnumsStructs.hpp"
#include "I2cConfigPolicy.hpp"
#include <functional>

using namespace Wrapper;
using namespace WrapperBase;

namespace Hal {

	/// @brief Interface pour le driver I2C de bas niveau.
	struct II2cDriver {
		template<I2cConfigPolicy T>
			int8_t init();

		virtual HAL_StatusTypeDef master_transmit(int8_t handleIndex, uint16_t devAddress, uint8_t* data, uint16_t size, uint32_t timeout) = 0;
		virtual HAL_StatusTypeDef master_receive(int8_t handleIndex, uint16_t devAddress, uint8_t* data, uint16_t size, uint32_t timeout) = 0;
		virtual HAL_StatusTypeDef mem_write(int8_t handleIndex, uint16_t devAddress, uint16_t memAddress, uint16_t memAddSize, uint8_t* data, uint16_t size, uint32_t timeout) = 0;
		virtual HAL_StatusTypeDef mem_read(int8_t handleIndex, uint16_t devAddress, uint16_t memAddress, uint16_t memAddSize, uint8_t* data, uint16_t size, uint32_t timeout) = 0;

		virtual HAL_StatusTypeDef master_transmit_it(int8_t handleIndex, uint16_t devAddress, uint8_t* data, uint16_t size) = 0;
		virtual HAL_StatusTypeDef master_receive_it(int8_t handleIndex, uint16_t devAddress, uint8_t* data, uint16_t size) = 0;
		virtual HAL_StatusTypeDef mem_write_it(int8_t handleIndex, uint16_t devAddress, uint16_t memAddress, uint16_t memAddSize, uint8_t* data, uint16_t size) = 0;
		virtual HAL_StatusTypeDef mem_read_it(int8_t handleIndex, uint16_t devAddress, uint16_t memAddress, uint16_t memAddSize, uint8_t* data, uint16_t size) = 0;

		static constexpr I2C_TypeDef* MapPort(I2cPort port);
		static constexpr uint32_t MapAddressingMode(I2cAddressingMode mode);
		static constexpr uint32_t MapDutyCycle(I2cDutyCycle cycle);

		static void attach_callbacks(int8_t handle_index,
			std::function<void()> tx_cb,
			std::function<void()> rx_cb,
			std::function<void()> error_cb);
                                     
		static void activate_IRQ(I2cPort port);
		static void enable_clock(I2cPort port);
		static bool is_enabled(I2cPort port);

		virtual ~II2cDriver() = default;
	};

} // namespace Hal