#pragma once

#include <cstdint>

namespace WrapperBase {

	/// <summary>
	/// @brief Énumération des périphériques ADC disponibles.
	/// </summary>
	enum class AdcPort {
		ADC_1,
		ADC_2,
		ADC_3
		// Ajoutez d'autres si votre MCU en a plus
	};

	/// <summary>
	/// @brief Énumération des canaux ADC (y compris les canaux internes).
	/// </summary>
	enum class AdcChannel {
		Channel_0,
		Channel_1,
		Channel_2,
		Channel_3,
		Channel_4,
		Channel_5,
		Channel_6,
		Channel_7,
		Channel_8,
		Channel_9,
		Channel_10,
		Channel_11,
		Channel_12,
		Channel_13,
		Channel_14,
		Channel_15,
		Channel_16,
		Channel_17,
		Channel_18,
		TempSensor,
		// Capteur de température interne
		VRefInt, // Référence de tension interne
		VBat        // Mesure de la batterie
	};

	/// <summary>
	/// @brief Résolution de la conversion ADC.
	/// </summary>
	enum class AdcResolution {
		Res_12bit = 0,
		// 12 bits
		Res_10bit = 1,
		// 10 bits
		Res_8bit = 2,
		// 8 bits
		Res_6bit = 3  // 6 bits
	};

	/// <summary>
	/// @brief Temps d'échantillonnage pour un canal.
	/// </summary>
	enum class AdcSampleTime {
		Cycles_3,
		Cycles_15,
		Cycles_28,
		Cycles_56,
		Cycles_84,
		Cycles_112,
		Cycles_144,
		Cycles_480
	};

	/// <summary>
	/// @brief Structure de configuration statique pour un canal ADC.
	/// </summary>
	template <
	    AdcPort port = AdcPort::ADC_1,
	    AdcChannel channel = AdcChannel::Channel_0,
	    AdcResolution res = AdcResolution::Res_12bit,
	    AdcSampleTime time = AdcSampleTime::Cycles_3
	    // On pourrait ajouter d'autres options : mode continu, trigger, etc.
	>
		struct AdcStaticConfig {
			static constexpr AdcPort Port = port;
			static constexpr AdcChannel Channel = channel;
			static constexpr AdcResolution Resolution = res;
			static constexpr AdcSampleTime SampleTime = time;

			// Pour l'instant, CanRead est toujours vrai pour un ADC
			static constexpr bool CanRead = true; 
		};

} // namespace WrapperBase