// UartDriver.hpp
#pragma once

#include <vector>
#include <cstdint>
#include <map>
#include <functional>
#include "IUartDriver.hpp"

namespace Hal {

	struct HalUartDriver : public Hal::IUartDriver {
		
		inline static constexpr int8_t handleEmpty = -1;
		
		inline static std::vector<UART_HandleTypeDef> uartHandles = { };
		inline static std::map<USART_TypeDef*, std::function<void()>> tx_complete_callbacks = { };
		inline static std::map<USART_TypeDef*, std::function<void()>> rx_complete_callbacks = { };
		
		template<typename T>
		int8_t init() {

			enable_clock(T::Port);

			UART_HandleTypeDef m_handle = {};
			
			// Configurer le handle UART
			m_handle.Instance = MapPort(T::Port);
			m_handle.Init = getHALConfig<T>();

			HAL_StatusTypeDef status = HAL_UART_Init(&m_handle);
        
			if (status == HAL_OK) {
				uartHandles.push_back(m_handle);
				return uartHandles.size() - 1;
			}
			return handleEmpty;
		}
		
		static USART_TypeDef* uart_instance(int8_t handle_index) {
			return uartHandles[handle_index].Instance;
		}
		
		static void attach_callbacks(int8_t handle_index, std::function<void()> tx_cb, std::function<void()> rx_cb) {
			USART_TypeDef* uart_instance = uartHandles[handle_index].Instance;
			if (tx_cb) {
				tx_complete_callbacks[uart_instance] = std::move(tx_cb);
			}
			if (rx_cb) {
				rx_complete_callbacks[uart_instance] = std::move(rx_cb);
			}
		}
		
		static void handle_tx_complete(UART_HandleTypeDef *huart) {
			if (tx_complete_callbacks.count(huart->Instance)) {
				tx_complete_callbacks[huart->Instance]();
			}
		}

		static void handle_rx_complete(UART_HandleTypeDef *huart) {
			if (rx_complete_callbacks.count(huart->Instance)) {
				rx_complete_callbacks[huart->Instance]();
			}
		}
		
		static void activate_IRQ(UartPort port) {

			// Activer les IRQs
			IRQn_Type irq;
			switch (port) {
				case UartPort::USART_1: irq = USART1_IRQn; break;
				case UartPort::USART_2: irq = USART2_IRQn;  break;
				case UartPort::USART_3: irq = USART3_IRQn;  break;
				case UartPort::UART_4:  irq = UART4_IRQn;  break;
				case UartPort::UART_5:  irq = UART5_IRQn;  break;
				case UartPort::USART_6: irq = USART6_IRQn;  break;
			}
        
			HAL_NVIC_SetPriority(irq, 0, 0);
			HAL_NVIC_EnableIRQ(irq);
		}
		
		HAL_StatusTypeDef transmit(int8_t handleIndex, const uint8_t* data, uint16_t size, uint32_t timeout) override {
			return HAL_UART_Transmit(&uartHandles[handleIndex], data, size, timeout);
		}

		HAL_StatusTypeDef receive(int8_t handleIndex, uint8_t* data, uint16_t size, uint32_t timeout) override {
			return HAL_UART_Receive(&uartHandles[handleIndex], data, size, timeout);
		}

		HAL_StatusTypeDef transmit_it(int8_t handleIndex, const uint8_t* data, uint16_t size) override {
			return HAL_UART_Transmit_IT(&uartHandles[handleIndex], data, size);
		}

		HAL_StatusTypeDef receive_it(int8_t handleIndex, uint8_t* data, uint16_t size) override {
			return HAL_UART_Receive_IT(&uartHandles[handleIndex], data, size);
		}
		
		static void enable_clock(UartPort port) {
			if (is_enabled(port))
				return;
			
			// Activer l'horloge de l'UART/USART
			switch (port) {
				case UartPort::USART_1: __HAL_RCC_USART1_CLK_ENABLE(); break;
				case UartPort::USART_2: __HAL_RCC_USART2_CLK_ENABLE(); break;
				case UartPort::USART_3: __HAL_RCC_USART3_CLK_ENABLE(); break;
				case UartPort::UART_4:  __HAL_RCC_UART4_CLK_ENABLE(); break;
				case UartPort::UART_5:  __HAL_RCC_UART5_CLK_ENABLE(); break;
				case UartPort::USART_6: __HAL_RCC_USART6_CLK_ENABLE(); break;
			}
		}
		
		static bool is_enabled(UartPort port) {
			switch (port) {
				case UartPort::USART_1: return RCC->APB2ENR & RCC_APB2ENR_USART1EN;
				case UartPort::USART_2: return RCC->APB1ENR & RCC_APB1ENR_USART2EN;
				case UartPort::USART_3: return RCC->APB1ENR & RCC_APB1ENR_USART3EN;
				case UartPort::UART_4:  return RCC->APB1ENR & RCC_APB1ENR_UART4EN;
				case UartPort::UART_5:  return RCC->APB1ENR & RCC_APB1ENR_UART5EN;
				case UartPort::USART_6: return RCC->APB2ENR & RCC_APB2ENR_USART6EN;
			}
			return false;
		}
		
		static constexpr USART_TypeDef* MapPort(UartPort port) {
			switch (port) {
			case UartPort::USART_1: return USART1;
			case UartPort::USART_2: return USART2;
			case UartPort::USART_3: return USART3;
			case UartPort::UART_4:  return UART4;
			case UartPort::UART_5:  return UART5;
			case UartPort::USART_6: return USART6;
			}
			return nullptr; 
		}
		
		/*
		* Fonctions de mappage vers les constantes HAL.
		**/
		static constexpr uint32_t MapWordLength(UartWordLength len) {
			return (len == UartWordLength::Data9Bits) ? UART_WORDLENGTH_9B : UART_WORDLENGTH_8B;
		}

		static constexpr uint32_t MapStopBits(UartStopBits sb) {
			return (sb == UartStopBits::Stop2) ? UART_STOPBITS_2 : UART_STOPBITS_1;
		}

		static constexpr uint32_t MapParity(UartParity p) {
			if (p == UartParity::Even) return UART_PARITY_EVEN;
			if (p == UartParity::Odd) return UART_PARITY_ODD;
			return UART_PARITY_NONE;
		}

		static constexpr uint32_t MapMode(UartMode m) {
			if (m == UartMode::Rx) return UART_MODE_RX;
			if (m == UartMode::Tx) return UART_MODE_TX;
			return UART_MODE_TX_RX;
		}

		static constexpr uint32_t MapHwControl(UartHwControl hc) {
			if (hc == UartHwControl::Rts) return UART_HWCONTROL_RTS;
			if (hc == UartHwControl::Cts) return UART_HWCONTROL_CTS;
			if (hc == UartHwControl::RtsCts) return UART_HWCONTROL_RTS_CTS;
			return UART_HWCONTROL_NONE;
		}
		
		template<typename config>
		static constexpr UART_InitTypeDef getHALConfig() {
			UART_InitTypeDef uartConfig {}
			;
			uartConfig.BaudRate            = config::BaudRate;
			uartConfig.WordLength          = MapWordLength(config::WordLength);
			uartConfig.StopBits            = MapStopBits(config::StopBits);
			uartConfig.Parity              = MapParity(config::Parity);
			uartConfig.Mode                = MapMode(config::Mode);
			uartConfig.HwFlowCtl           = MapHwControl(config::HwControl);
			uartConfig.OverSampling        = config::Oversampling == UartOversampling::OverSampling_16 ? UART_OVERSAMPLING_16 : UART_OVERSAMPLING_8;
			return uartConfig;
		}
		
	};

} // namespace Driver