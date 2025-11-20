#pragma once

#include <cstdint>
#include "I2cEnumsStructs.hpp"
#include "II2cDriver.hpp"
#include "I2cDriver.hpp"
#include "GpioStatic.hpp" // Crucial : pour initialiser les broches
#include "I2cConfigPolicy.hpp"

using namespace Hal;

namespace Wrapper {

	template<I2cConfigPolicy config, typename Driver = HalI2cDriver>
		class I2cStatic {
		public:
			I2cStatic()
				: handleIndex(HalI2cDriver::handleEmpty) {}

			void init() {
				// 1. Initialiser les broches GPIO SCL et SDA
				// C'est ici que la "magie" de la composition opère
				m_scl_pin.init();
				m_sda_pin.init();
            
				// 2. Initialiser le périphérique I2C
				int8_t handleindex = m_driver.template init<config>();
                
				if (handleindex != HalI2cDriver::handleEmpty) {
					handleIndex = handleindex;
				}
			}

			// --- Fonctions de communication bloquantes ---

			HAL_StatusTypeDef master_transmit(uint16_t devAddress, const uint8_t* data, uint16_t size, uint32_t timeout = HAL_MAX_DELAY) {
				// L'adresse I2C est souvent 7 bits, elle doit être décalée de 1.
				// La HAL gère cela si l'adresse est passée non décalée.
				return m_driver.master_transmit(handleIndex, devAddress, data, size, timeout);
			}

			HAL_StatusTypeDef master_receive(uint16_t devAddress, uint8_t* data, uint16_t size, uint32_t timeout = HAL_MAX_DELAY) {
				return m_driver.master_receive(handleIndex, devAddress, data, size, timeout);
			}

			HAL_StatusTypeDef mem_write(uint16_t devAddress, uint16_t memAddress, uint16_t memAddSize, const uint8_t* data, uint16_t size, uint32_t timeout = HAL_MAX_DELAY) {
				return m_driver.mem_write(handleIndex, devAddress, memAddress, memAddSize, data, size, timeout);
			}

			HAL_StatusTypeDef mem_read(uint16_t devAddress, uint16_t memAddress, uint16_t memAddSize, uint8_t* data, uint16_t size, uint32_t timeout = HAL_MAX_DELAY) {
				return m_driver.mem_read(handleIndex, devAddress, memAddress, memAddSize, data, size, timeout);
			}

			// --- Fonctions de communication par interruption ---

			HAL_StatusTypeDef master_transmit_it(uint16_t devAddress, const uint8_t* data, uint16_t size) {
				return m_driver.master_transmit_it(handleIndex, devAddress, data, size);
			}

			HAL_StatusTypeDef master_receive_it(uint16_t devAddress, uint8_t* data, uint16_t size) {
				return m_driver.master_receive_it(handleIndex, devAddress, data, size);
			}
        
			HAL_StatusTypeDef mem_write_it(uint16_t devAddress, uint16_t memAddress, uint16_t memAddSize, const uint8_t* data, uint16_t size) {
				return m_driver.mem_write_it(handleIndex, devAddress, memAddress, memAddSize, data, size);
			}

			HAL_StatusTypeDef mem_read_it(uint16_t devAddress, uint16_t memAddress, uint16_t memAddSize, uint8_t* data, uint16_t size) {
				return m_driver.mem_read_it(handleIndex, devAddress, memAddress, memAddSize, data, size);
			}

			// --- Gestion des Callbacks ---

			void attach_callbacks(std::function<void()> tx_cb, std::function<void()> rx_cb, std::function<void()> error_cb) {
				HalI2cDriver::activate_IRQ(config::Port);
				HalI2cDriver::attach_callbacks(handleIndex, std::move(tx_cb), std::move(rx_cb), std::move(error_cb));
			}

		private:
			Driver m_driver;
			int8_t handleIndex;
        
			// Instanciation des wrappers GpioStatic pour SCL et SDA
			// Le type de ces membres est déterminé par la "Policy"
			typename config::SclPin::template GpioStatic<typename config::SclPin> m_scl_pin;
			typename config::SdaPin::template GpioStatic<typename config::SdaPin> m_sda_pin;
		};

} // namespace Wrapper