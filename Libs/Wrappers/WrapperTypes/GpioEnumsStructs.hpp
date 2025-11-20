#pragma once

#include <cstdbool>
#include <initializer_list>
#include <cstdint>
#include <concepts>

namespace WrapperBase {

	/*
	 * @brief Énumération des différents modes de bus pouvant être utilisés pour communiquer avec le matériel.
	 *		Input			  : Input Floating Mode,
	 *		Output			  : Output Push Pull Mode
	 *		AlternateFunction : Alternate Function Push Pull Mode
	 *		Analog			  : Analog Mode
	 **/
    enum class GpioPinMode
    {
        //None                = -1,
        Input               = 0,
        Output              = 1,
        AlternateFunction   = 2,
        Analog              = 3,
    };
    
	/*
	 * @brief Énumération des différentes options de résistance pull-up/pull-down pour une broche GPIO.
	 **/
    enum class GpioPullMode {
        None,
        Up,
        Down
    };
	
	/*
	 * @brief Énumération des différentes vitesses de bus pouvant être utilisés pour communiquer avec le matériel.
	 * Les valeurs sont arbitraires et dépendent du matériel.
	 * Les valeurs sont classées par ordre croissant de vitesse.
	 *		Low : Jusqu'à 10MHz pour Gpio, Jusqu'à 100kHz pour I2c, Jusqu'à 1MHz pour Spi, Jusqu'à 1MHz pour Can, Jusqu'à 1MHz pour Sitl, Jusqu'à 115200bps pour Msp, Jusqu'à 115200bps pour Serial, Jusqu'à 50MHz pour Wspi
	 *		Medium : Jusqu'à 50MHz pour Gpio, Jusqu'à 400kHz pour I2c, Jusqu'à 10MHz pour Spi, Jusqu'à 1MHz pour Can, Jusqu'à 1MHz pour Sitl, Jusqu'à 115200bps pour Msp, Jusqu'à 115200bps pour Serial, Jusqu'à 50MHz pour Wspi
	 *		High : Jusqu'à 100MHz pour Gpio, Jusqu'à 1MHz pour I2c, Jusqu'à 20MHz pour Spi, Jusqu'à 1MHz pour Can, Jusqu'à 1MHz pour Sitl, Jusqu'à 115200bps pour Msp, Jusqu'à 115200bps pour Serial, Jusqu'à 50MHz pour Wspi
	 *		VeryHigh : Jusqu'à 200MHz pour Gpio, Jusqu'à 3.4MHz pour I2c, Jusqu'à 40MHz pour Spi, Jusqu'à 1MHz pour Can, Jusqu'à 1MHz pour Sitl, Jusqu'à 115200bps pour Msp, Jusqu'à 115200bps pour Serial, Jusqu'à 50MHz pour Wspi
	 **/
    enum class GpioPinSpeed {
        Low,
        Medium,
        High,
        VeryHigh,
    };

	/*
	 * @brief Énumération des états possibles d'une broche GPIO.
	 **/
    enum class GpioPinState {
        Reset = 0, ///< État bas.
        Set = 1    ///< État haut.
    };

	/*
	 * @brief enum pour représenter un port GPIO.
	 **/
	enum class GpioPort {
		GPIO_A,
		GPIO_B,
		GPIO_C,
		GPIO_D,
		GPIO_E,
		GPIO_F,
		GPIO_G,
		GPIO_H,
		GPIO_I,
	};

	/*
	 * @brief Énumération des différents types de fonctions alternatives pour une broche GPIO.
	 **/
	enum class GpioAlternateFunctionType {
		None = 0,
		AF0_RTC_50Hz, AF0_MCO, AF0_TAMPER, AF0_SWJ, AF0_TRACE,
		AF1_TIM1, AF1_TIM2,
		AF2_TIM3, AF2_TIM4, AF2_TIM5,
		AF3_TIM8, AF3_TIM9, AF3_TIM10, AF3_TIM11,
		AF4_I2C1, AF4_I2C2, AF4_I2C3,
		AF5_SPI1, AF5_SPI2,
		AF6_SPI3,
		AF7_USART1, AF7_USART2, AF7_USART3,
		AF8_UART4, AF8_UART5, AF8_USART6,
		AF9_CAN1, AF9_CAN2, AF9_TIM12, AF9_TIM13, AF9_TIM14,
		AF10_OTG_FS, AF10_OTG_HS,
		AF11_ETH,
		AF12_FSMC, AF12_OTG_HS_FS, AF12_SDIO,
		AF13_DCMI,
		AF15_EVENTOUT,
	};
	
	/*
	 * @brief Énumération des différents types d'interruptions possibles pour une broche GPIO.
	 *		None : Pas d'interruption, pas de détection
	 *		Rising : Front montant, transition de bas en haut (0→1)
	 *		Falling : Front descendant, transition de haut en bas (1→0)
	 *		RisingFalling : Front montant et descendant, les deux transitions (0→1 et 1→0)
	 **/
	enum class GpioInterruptEdge {
		None, ///< No interrupt : No edge detection
		Rising, ///< Rising edge : Low to high transition(0→1)
		Falling, ///< Falling edge : High to low transition(1→0)
		RisingFalling, ///< Rising and falling edge : Both transitions(0→1 and 1→0)
	};

	/*
	 * @Brief structure de configuration Gpio
	 **/
	template <GpioPort port = GpioPort::GPIO_A, uint8_t pin = 0, GpioPinMode mode = GpioPinMode::Output, 
		GpioPullMode pull = GpioPullMode::None, GpioPinSpeed speed = GpioPinSpeed::Low, GpioInterruptEdge interrupt = GpioInterruptEdge::None,
		GpioAlternateFunctionType gpioAF = GpioAlternateFunctionType::None>
	struct GpioStaticConfig {
		static constexpr GpioPort Port = port;
		static constexpr uint8_t Pin = pin;
		static constexpr uint16_t PinMask = (1u << pin);
		static constexpr GpioPinMode Mode = mode;
		static constexpr GpioPullMode Pull = pull;
		static constexpr GpioPinSpeed Speed = speed;
		static constexpr GpioInterruptEdge Interrupt = interrupt;
		static constexpr GpioAlternateFunctionType GpioAF = gpioAF;

		static constexpr bool CanSet = (mode == GpioPinMode::Output || mode == GpioPinMode::AlternateFunction);
		static constexpr bool CanRead = (mode == GpioPinMode::Input);
		static constexpr bool CanToggle = (mode == GpioPinMode::Output);
	};
} // namespace Wrapper
