// SpiEnumsStructs.hpp
#pragma once

#include <cstdint>
#include "stm32f4xx_hal.h"
#include "GpioEnumsStructs.hpp" // Pour la composition

namespace Wrapper {

	/// @brief Énumération des ports SPI disponibles.
	enum class SpiPort {
		SPI_1,
		SPI_2,
		SPI_3,
		// SPI_4, 5, 6 existent sur certains F4, mais F407 a 1, 2, 3
	};

	/// @brief Mode de fonctionnement (Maître ou Esclave).
	enum class SpiMode {
		Master,
		Slave
	};

	/// @brief Direction de la communication SPI.
	enum class SpiDirection {
		FullDuplex, // 2 lignes (MISO, MOSI)
		HalfDuplex, // 1 ligne (MOSI bidirectionnelle)
		RxOnly        // 2 lignes, mais MOSI non utilisé
	};

	/// @brief Taille des données SPI.
	enum class SpiDataSize {
		Data8Bit,
		Data16Bit
	};

	/// @brief Polarité de l'horloge (CPOL).
	enum class SpiClockPolarity {
		Low, // Horloge à 0 au repos
		High  // Horloge à 1 au repos
	};

	/// @brief Phase de l'horloge (CPHA).
	enum class SpiClockPhase {
		Edge1,
		// Capture sur le 1er front d'horloge
		Edge2  // Capture sur le 2ème front d'horloge
	};

	/// @brief Gestion du signal "NSS" (Chip Select).
	enum class SpiNssMode {
		Software, // NSS géré manuellement par un GPIO (cas le plus courant)
		HardwareMaster, // NSS en sortie, géré par le matériel
		HardwareSlave    // NSS en entrée, géré par le matériel
	};

	/// @brief Prescaler pour le Baud Rate SPI.
	enum class SpiBaudRatePrescaler {
		Prescaler2,
		Prescaler4,
		Prescaler8,
		Prescaler16,
		Prescaler32,
		Prescaler64,
		Prescaler128,
		Prescaler256
	};

	/// @brief Ordre des bits (MSB ou LSB en premier).
	enum class SpiFirstBit {
		MSBFirst,
		LSBFirst
	};

	/**
	 * @brief Structure de configuration statique pour un périphérique SPI.
	 * @tparam SckPinConfig  Broche GPIO pour SCK (Horloge)
	 * @tparam MisoPinConfig Broche GPIO pour MISO (Master In Slave Out)
	 * @tparam MosiPinConfig Broche GPIO pour MOSI (Master Out Slave In)
	 * @tparam NssPinConfig  Broche GPIO pour NSS (Hardware Mode)
	 * Mettre 'void' si SpiNssMode::Software est utilisé.
	 * @note Les broches GPIO doivent être en Mode AlternateFunction avec le bon AF mapping.
	 */
	template <
	    typename SckPinConfig,
	    typename MisoPinConfig,
	    typename MosiPinConfig,
	    typename NssPinConfig = void, // 'void' si non utilisé (ex: Software NSS)
	    SpiPort port = SpiPort::SPI_1,
	    SpiMode mode = SpiMode::Master,
	    SpiDirection direction = SpiDirection::FullDuplex,
	    SpiDataSize dataSize = SpiDataSize::Data8Bit,
	    SpiClockPolarity cpol = SpiClockPolarity::Low,
	    SpiClockPhase cpha = SpiClockPhase::Edge1,
	    SpiNssMode nss = SpiNssMode::Software,
	    SpiBaudRatePrescaler prescaler = SpiBaudRatePrescaler::Prescaler16,
	    SpiFirstBit firstBit = SpiFirstBit::MSBFirst
	>
		struct SpiStaticConfig {
			using SckPin  = SckPinConfig;
			using MisoPin = MisoPinConfig;
			using MosiPin = MosiPinConfig;
			using NssPin  = NssPinConfig; // Peut être 'void'

			static constexpr SpiPort Port = port;
			static constexpr SpiMode Mode = mode;
			static constexpr SpiDirection Direction = direction;
			static constexpr SpiDataSize DataSize = dataSize;
			static constexpr SpiClockPolarity ClockPolarity = cpol;
			static constexpr SpiClockPhase ClockPhase = cpha;
			static constexpr SpiNssMode Nss = nss;
			static constexpr SpiBaudRatePrescaler Prescaler = prescaler;
			static constexpr SpiFirstBit FirstBit = firstBit;
		};

} // namespace Wrapper