#pragma once

#include <cstdint>
#include "stm32f4xx_hal.h"
#include "GpioEnumsStructs.hpp" // Important pour la composition

namespace Wrapper {

	/// @brief Énumération des ports I2C disponibles.
	enum class I2cPort {
		I2C_1,
		I2C_2,
		I2C_3,
	};

	/// @brief Énumération pour le mode d'adressage I2C.
	enum class I2cAddressingMode {
		Mode7Bit,
		Mode10Bit
	};

	/// @brief Énumération pour le "Duty Cycle" en mode rapide I2C.
	enum class I2cDutyCycle {
		Cycle2, // 2/3 duty cycle
		Cycle16_9     // 16/9 duty cycle
	};

	/// @brief Structure de configuration statique pour un périphérique I2C.
	/// @tparam SclPinConfig Configuration statique de la broche GPIO pour SCL.
	/// @tparam SdaPinConfig Configuration statique de la broche GPIO pour SDA.
	/// @note Les configurations GPIO doivent être en mode AlternateFunction,
	///       OpenDrain, PullUp, et avec le bon AF mapping (ex: AF4_I2C1).
	template <
	    typename SclPinConfig,
	    typename SdaPinConfig,
	    I2cPort port,
	    uint32_t clockSpeed,
	    I2cAddressingMode addrMode = I2cAddressingMode::Mode7Bit,
	    I2cDutyCycle dutyCycle = I2cDutyCycle::Cycle2,
	    uint32_t ownAddress1 = 0,
	    bool generalCallMode = false,
	    bool noStretchMode = false,
	    uint32_t ownAddress2 = 0,
	    bool dualAddressMode = false
	>
		struct I2cStaticConfig {
			using SclPin = SclPinConfig;
			using SdaPin = SdaPinConfig;

			static constexpr I2cPort Port = port;
			static constexpr uint32_t ClockSpeed = clockSpeed;
			static constexpr I2cAddressingMode AddressingMode = addrMode;
			static constexpr I2cDutyCycle DutyCycle = dutyCycle;
			static constexpr uint32_t OwnAddress1 = ownAddress1;
			static constexpr bool GeneralCallMode = generalCallMode;
			static constexpr bool NoStretchMode = noStretchMode;
			static constexpr uint32_t OwnAddress2 = ownAddress2;
			static constexpr bool DualAddressMode = dualAddressMode;
		};

} // namespace Wrapper