#pragma once

#include <cstdint>

namespace WrapperBase {

	/// <summary>
	/// @brief Énumération des périphériques DAC disponibles.
	/// (Les STM32F4 ont souvent un seul bloc DAC avec 2 canaux)
	/// </summary>
	enum class DacPort {
		DAC_1
	};

	/// <summary>
	/// @brief Énumération des canaux DAC.
	/// </summary>
	enum class DacChannel {
		Channel_1,
		// Associé à PA4 sur beaucoup de F4
		Channel_2  // Associé à PA5 sur beaucoup de F4
	};

	/// <summary>
	/// @brief Alignement des données en écriture (8 ou 12 bits).
	/// </summary>
	enum class DacDataAlign {
		Align_12b_Right,
		Align_12b_Left,
		Align_8b_Right
	};

	/// <summary>
	/// @brief Structure de configuration statique pour un canal DAC.
	/// </summary>
	template <
	    DacPort port = DacPort::DAC_1,
	    DacChannel channel = DacChannel::Channel_1,
	    DacDataAlign align = DacDataAlign::Align_12b_Right,
	    bool buffered = true // Activer ou non le buffer de sortie
	>
		struct DacStaticConfig {
			static constexpr DacPort Port = port;
			static constexpr DacChannel Channel = channel;
			static constexpr DacDataAlign Align = align;
			static constexpr bool OutputBuffer = buffered;

			// On peut écrire sur un DAC, mais pas le lire (pas directement)
			static constexpr bool CanSet = true;
			static constexpr bool CanRead = false; 
		};

} // namespace WrapperBase