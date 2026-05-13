// UartStatic.hpp
#pragma once

#include <cstdint>
#include "UartEnumsStructs.hpp"
#include "IUartDriver.hpp"
#include "UartDriver.hpp"
#include "GpioStatic.hpp" // Pour initialiser les broches
#include "UartConfigPolicy.hpp"

using namespace Hal;

namespace Wrapper {
	template<UartConfigPolicy config, typename Driver = HalUartDriver>
		class UartStatic {
		public:
			UartStatic()
				: handleIndex(HalUartDriver::handleEmpty) {}

			void init() {

				// Initialiser les broches GPIO
				m_tx_pin.init();
				m_rx_pin.init();
				
				int8_t handleindex = m_driver.template init<config>();
					
				if (handleindex != HalUartDriver::handleEmpty) {
					handleIndex = handleindex;
				}
			}

			DriverStatus transmit(const uint8_t* data, uint16_t size, uint32_t timeout = HAL_MAX_DELAY) {
				return m_driver.transmit(handleIndex, data, size, timeout);
			}

			DriverStatus receive(uint8_t* data, uint16_t size, uint32_t timeout = HAL_MAX_DELAY) {
				return m_driver.receive(handleIndex, data, size, timeout);
			}
    
			DriverStatus transmit_it(const uint8_t* data, uint16_t size) {
				return m_driver.transmit_it(handleIndex, data, size);
			}

			DriverStatus receive_it(uint8_t* data, uint16_t size) {
				return m_driver.receive_it(handleIndex, data, size);
			}

			void attach_callbacks(std::function<void()> tx_cb, std::function<void()> rx_cb) {
				HalUartDriver::activate_IRQ(config::Port, uint32_t preempt, uint32_t sub);
				HalUartDriver::attach_callbacks(handleIndex, std::move(tx_cb), std::move(rx_cb));
			}


		private:
			Driver m_driver;

			int8_t handleIndex;
			
			Wrapper::GpioStatic<typename config::TxPin> m_tx_pin;
			Wrapper::GpioStatic<typename config::RxPin> m_rx_pin;
		};
	
}
