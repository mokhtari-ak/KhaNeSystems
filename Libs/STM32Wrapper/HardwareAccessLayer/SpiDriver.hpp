// SpiDriver.hpp
#pragma once

#include <vector>
#include <cstdint>
#include <map>
#include <functional>
#include "ISpiDriver.hpp"

namespace Hal {

	struct HalSpiDriver : public Hal::ISpiDriver {

		inline static constexpr int8_t handleEmpty = -1;
		inline static std::vector<SPI_HandleTypeDef> spiHandles = { };

		// Maps pour les callbacks
		inline static std::map<SPI_TypeDef*, std::function<void()>> tx_complete_callbacks = { };
		inline static std::map<SPI_TypeDef*, std::function<void()>> rx_complete_callbacks = { };
		inline static std::map<SPI_TypeDef*, std::function<void()>> txrx_complete_callbacks = { };
		inline static std::map<SPI_TypeDef*, std::function<void()>> error_callbacks = { };

		template<SpiConfigPolicy T>
			int8_t init() {
				enable_clock(T::Port);

				SPI_HandleTypeDef m_handle = { };
				m_handle.Instance = MapPort(T::Port);
				m_handle.Init = getHALConfig<T>();

				HAL_StatusTypeDef status = HAL_SPI_Init(&m_handle);

				if (status == HAL_OK) {
					spiHandles.push_back(m_handle);
					return spiHandles.size() - 1;
				}
				return handleEmpty;
			}

		// --- Implémentation des fonctions bloquantes ---
		HAL_StatusTypeDef transmit(int8_t handleIndex, const uint8_t* data, uint16_t size, uint32_t timeout) override {
			return HAL_SPI_Transmit(&spiHandles[handleIndex], data, size, timeout);
		}
		HAL_StatusTypeDef receive(int8_t handleIndex, uint8_t* data, uint16_t size, uint32_t timeout) override {
			return HAL_SPI_Receive(&spiHandles[handleIndex], data, size, timeout);
		}
		HAL_StatusTypeDef transmit_receive(int8_t handleIndex, const uint8_t* txData, uint8_t* rxData, uint16_t size, uint32_t timeout) override {
			return HAL_SPI_TransmitReceive(&spiHandles[handleIndex], txData, rxData, size, timeout);
		}

		// --- Implémentation des fonctions non-bloquantes (IT) ---
		HAL_StatusTypeDef transmit_it(int8_t handleIndex, const uint8_t* data, uint16_t size) override {
			return HAL_SPI_Transmit_IT(&spiHandles[handleIndex], data, size);
		}
		HAL_StatusTypeDef receive_it(int8_t handleIndex, uint8_t* data, uint16_t size) override {
			return HAL_SPI_Receive_IT(&spiHandles[handleIndex], data, size);
		}
		HAL_StatusTypeDef transmit_receive_it(int8_t handleIndex, const uint8_t* txData, uint8_t* rxData, uint16_t size) override {
			return HAL_SPI_TransmitReceive_IT(&spiHandles[handleIndex], txData, rxData, size);
		}

		// --- Fonctions statiques de gestion ---
		static void attach_callbacks(int8_t handle_index,
			std::function<void()> tx_cb,
			std::function<void()> rx_cb,
			std::function<void()> txrx_cb,
			std::function<void()> error_cb) {
			SPI_TypeDef* instance = spiHandles[handle_index].Instance;
			if (tx_cb) tx_complete_callbacks[instance] = std::move(tx_cb);
			if (rx_cb) rx_complete_callbacks[instance] = std::move(rx_cb);
			if (txrx_cb) txrx_complete_callbacks[instance] = std::move(txrx_cb);
			if (error_cb) error_callbacks[instance] = std::move(error_cb);
		}

		// Fonctions à appeler depuis les callbacks globaux HAL (stm32f4xx_it.c)
		static void handle_tx_complete(SPI_HandleTypeDef *hspi) {
			if (tx_complete_callbacks.count(hspi->Instance)) {
				tx_complete_callbacks[hspi->Instance]();
			}
		}
		static void handle_rx_complete(SPI_HandleTypeDef *hspi) {
			if (rx_complete_callbacks.count(hspi->Instance)) {
				rx_complete_callbacks[hspi->Instance]();
			}
		}
		static void handle_txrx_complete(SPI_HandleTypeDef *hspi) {
			if (txrx_complete_callbacks.count(hspi->Instance)) {
				txrx_complete_callbacks[hspi->Instance]();
			}
		}
		static void handle_error(SPI_HandleTypeDef *hspi) {
			if (error_callbacks.count(hspi->Instance)) {
				error_callbacks[hspi->Instance]();
			}
		}

		static void activate_IRQ(SpiPort port) {
			IRQn_Type irq;
			switch (port) {
			case SpiPort::SPI_1: irq = SPI1_IRQn; break;
			case SpiPort::SPI_2: irq = SPI2_IRQn; break;
			case SpiPort::SPI_3: irq = SPI3_IRQn; break;
			}
			HAL_NVIC_SetPriority(irq, 0, 0);
			HAL_NVIC_EnableIRQ(irq);
		}

		static void enable_clock(SpiPort port) {
			if (is_enabled(port)) return;
			switch (port) {
				// SPI1 est sur APB2
			case SpiPort::SPI_1: __HAL_RCC_SPI1_CLK_ENABLE(); break;
				// SPI2 et SPI3 sont sur APB1
			case SpiPort::SPI_2: __HAL_RCC_SPI2_CLK_ENABLE(); break;
			case SpiPort::SPI_3: __HAL_RCC_SPI3_CLK_ENABLE(); break;
			}
		}

		static bool is_enabled(SpiPort port) {
			switch (port) {
			case SpiPort::SPI_1: return RCC->APB2ENR & RCC_APB2ENR_SPI1EN;
			case SpiPort::SPI_2: return RCC->APB1ENR & RCC_APB1ENR_SPI2EN;
			case SpiPort::SPI_3: return RCC->APB1ENR & RCC_APB1ENR_SPI3EN;
			}
			return false;
		}

		// --- Fonctions de mappage statiques ---
		template<SpiConfigPolicy config>
			static constexpr SPI_InitTypeDef getHALConfig() {
				SPI_InitTypeDef spiConfig {}
				;
				spiConfig.Mode = MapMode(config::Mode);
				spiConfig.Direction = MapDirection(config::Direction);
				spiConfig.DataSize = MapDataSize(config::DataSize);
				spiConfig.CLKPolarity = MapClockPolarity(config::ClockPolarity);
				spiConfig.CLKPhase = MapClockPhase(config::ClockPhase);
				spiConfig.NSS = MapNssMode(config::Nss);
				spiConfig.BaudRatePrescaler = MapPrescaler(config::Prescaler);
				spiConfig.FirstBit = MapFirstBit(config::FirstBit);
				spiConfig.TIMode = SPI_TIMODE_DISABLE;
				spiConfig.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
				spiConfig.CRCPolynomial = 10;
				return spiConfig;
			}

		static constexpr SPI_TypeDef* MapPort(SpiPort port) {
			switch (port) {
			case SpiPort::SPI_1: return SPI1;
			case SpiPort::SPI_2: return SPI2;
			case SpiPort::SPI_3: return SPI3;
			}
			return nullptr;
		}
		static constexpr uint32_t MapMode(SpiMode mode) {
			return (mode == SpiMode::Master) ? SPI_MODE_MASTER : SPI_MODE_SLAVE;
		}
		static constexpr uint32_t MapDirection(SpiDirection dir) {
			if (dir == SpiDirection::HalfDuplex) return SPI_DIRECTION_1LINE;
			if (dir == SpiDirection::RxOnly) return SPI_DIRECTION_2LINES_RXONLY;
			return SPI_DIRECTION_2LINES;
		}
		static constexpr uint32_t MapDataSize(SpiDataSize size) {
			return (size == SpiDataSize::Data16Bit) ? SPI_DATASIZE_16BIT : SPI_DATASIZE_8BIT;
		}
		static constexpr uint32_t MapClockPolarity(SpiClockPolarity cpol) {
			return (cpol == SpiClockPolarity::High) ? SPI_POLARITY_HIGH : SPI_POLARITY_LOW;
		}
		static constexpr uint32_t MapClockPhase(SpiClockPhase cpha) {
			return (cpha == SpiClockPhase::Edge2) ? SPI_PHASE_2EDGE : SPI_PHASE_1EDGE;
		}
		static constexpr uint32_t MapNssMode(SpiNssMode nss) {
			if (nss == SpiNssMode::HardwareMaster) return SPI_NSS_HARD_OUTPUT;
			if (nss == SpiNssMode::HardwareSlave) return SPI_NSS_HARD_INPUT;
			return SPI_NSS_SOFT;
		}
		static constexpr uint32_t MapPrescaler(SpiBaudRatePrescaler psc) {
			switch (psc) {
			case SpiBaudRatePrescaler::Prescaler2:   return SPI_BAUDRATEPRESCALER_2;
			case SpiBaudRatePrescaler::Prescaler4:   return SPI_BAUDRATEPRESCALER_4;
			case SpiBaudRatePrescaler::Prescaler8:   return SPI_BAUDRATEPRESCALER_8;
			case SpiBaudRatePrescaler::Prescaler16:  return SPI_BAUDRATEPRESCALER_16;
			case SpiBaudRatePrescaler::Prescaler32:  return SPI_BAUDRATEPRESCALER_32;
			case SpiBaudRatePrescaler::Prescaler64:  return SPI_BAUDRATEPRESCALER_64;
			case SpiBaudRatePrescaler::Prescaler128: return SPI_BAUDRATEPRESCALER_128;
			case SpiBaudRatePrescaler::Prescaler256: return SPI_BAUDRATEPRESCALER_256;
			}
			return SPI_BAUDRATEPRESCALER_16;
		}
		static constexpr uint32_t MapFirstBit(SpiFirstBit fb) {
			return (fb == SpiFirstBit::LSBFirst) ? SPI_FIRSTBIT_LSB : SPI_FIRSTBIT_MSB;
		}
	};

} // namespace Hal