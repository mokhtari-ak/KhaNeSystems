#pragma once

#include "GpioEnumsStructs.hpp" // Réutilise vos enums GPIO
#include "GpioConfigPolicy.hpp" // Réutilise votre concept GPIO
#include <cstdint>

namespace WrapperBase {

	/// <summary>
	/// @brief Énumération des instances de Timer disponibles.
	/// (Liste pour STM32F407)
	/// </summary>
	enum class PwmTimerInstance {
		TIM1,
		TIM2,
		TIM3,
		TIM4,
		TIM5,
		TIM8,
		TIM9,
		TIM10,
		TIM11,
		TIM12,
		TIM13,
		TIM14
	};

	/// <summary>
	/// @brief Énumération des canaux du Timer.
	/// </summary>
	enum class PwmTimerChannel {
		Channel1,
		Channel2,
		Channel3,
		Channel4
	};

	/// <summary>
	/// @brief Polarité du signal PWM.
	/// </summary>
	enum class PwmPolarity {
		High,
		Low
	};

	/// <summary>
	/// @brief Mode PWM (PWM1 ou PWM2).
	/// PWM1 : Le canal est actif tant que TIMx_CNT < TIMx_CCR.
	/// PWM2 : Le canal est actif tant que TIMx_CNT > TIMx_CCR.
	/// </summary>
	enum class PwmMode {
		PWM1,
		PWM2
	};

	/*
	 * @Brief structure de configuration PWM statique
	 * @tparam T_GpioPolicy Une politique de configuration GPIO (doit être GpioConfigPolicy)
	 * Cette policy DOIT configurer la broche en Mode::AlternateFunction
	 * et avec le bon GpioAF (ex: GpioAlternateFunctionType::AF1_TIM2)
	 * @tparam timer Instance du timer (ex: PwmTimerInstance::TIM2)
	 * @tparam channel Canal du timer (ex: PwmTimerChannel::Channel1)
	 * @tparam prescaler Valeur du prescaler (PSC)
	 * @tparam period Valeur de l'auto-reload (ARR) (définit la fréquence)
	 * @tparam polarity Polarité du signal (ex: PwmPolarity::High)
	 * @tparam mode Mode PWM (ex: PwmMode::PWM1)
	 **/
	template <
	    GpioConfigPolicy T_GpioPolicy,
	    PwmTimerInstance timer,
	    PwmTimerChannel channel,
	    uint32_t prescaler,
	    uint32_t period, // C'est la valeur ARR (Auto-Reload Register)
	    PwmPolarity polarity = PwmPolarity::High,
	    PwmMode mode = PwmMode::PWM1
	>
		struct PwmStaticConfig {
			// La configuration GPIO pour la broche de sortie PWM
			using GpioPolicy = T_GpioPolicy;

			static constexpr PwmTimerInstance Timer = timer;
			static constexpr PwmTimerChannel Channel = channel;
        
			// Configuration de la base de temps
			static constexpr uint32_t Prescaler = prescaler;
			static constexpr uint32_t Period = period; // Valeur ARR

			// Configuration du canal
			static constexpr PwmPolarity Polarity = polarity;
			static constexpr PwmMode Mode = mode;
		};

} // namespace WrapperBase