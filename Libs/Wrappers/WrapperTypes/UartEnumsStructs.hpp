#pragma once

#include <cstdint>
#include "stm32f4xx_hal.h"
#include "GpioEnumsStructs.hpp" // Important pour la composition

namespace Wrapper {

	/*
	 * @brief Énumération des ports UART/USART disponibles.
	 **/
	enum class UartPort {
		USART_1,
		USART_2,
		USART_3,
		UART_4,
		UART_5,
		USART_6,
	};

	/*
	 * @brief Énumération pour la longueur des données.
	 **/
	enum class UartWordLength {
		Data8Bits,
		Data9Bits
	};

	/*
	 * @brief Énumération pour le nombre de bits d'arrêt.
	 **/
	enum class UartStopBits {
		Stop1,
		Stop2
	};

	/*
	 * @brief Énumération pour la parité.
	 *
	 **/
	enum class UartParity {
		None,
		Even,
		Odd
	};

	/*
	 * @brief Énumération pour le mode de fonctionnement (Tx, Rx, ou les deux).
	 **/
	enum class UartMode {
		Rx,
		Tx,
		TxRx
	};

	/*
	 * @brief Énumération pour le contrôle de flux matériel.
	 **/
	enum class UartHwControl {
		None,
		Rts,
		Cts,
		RtsCts
	};
	
	/*
	 * @brief Énumération pour le OverSampling
	 **/
	enum class UartOversampling {
		OverSampling_8,
		OverSampling_16
	};
	
	/*
	 * @brief Structure de configuration statique pour un périphérique UART.
	 * @tparam TxPinConfig Configuration statique de la broche GPIO pour Tx.
	 * @tparam RxPinConfig Configuration statique de la broche GPIO pour Rx.
	 * @note Les configurations GPIO doivent être en mode AlternateFunction avec le bon AF mapping.
	 **/
	template <
	    typename TxPinConfig, 
	    typename RxPinConfig,
	    UartPort port,
	    uint32_t baudrate,
	    UartWordLength wordLength = UartWordLength::Data8Bits,
	    UartStopBits stopBits = UartStopBits::Stop1,
	    UartParity parity = UartParity::None,
	    UartMode mode = UartMode::TxRx,
	    UartHwControl hwControl = UartHwControl::None,
		UartOversampling oversampling = UartOversampling::OverSampling_16
	>
		struct UartStaticConfig {
			using TxPin = TxPinConfig;
			using RxPin = RxPinConfig;

			static constexpr UartPort Port = port;
			static constexpr uint32_t BaudRate = baudrate;
			static constexpr UartWordLength WordLength = wordLength;
			static constexpr UartStopBits StopBits = stopBits;
			static constexpr UartParity Parity = parity;
			static constexpr UartMode Mode = mode;
			static constexpr UartHwControl HwControl = hwControl;
			static constexpr UartOversampling Oversampling = UartOversampling::OverSampling_16;
        
		};

} // namespace WrapperBase