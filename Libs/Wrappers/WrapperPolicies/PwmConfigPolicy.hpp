#pragma once

#include "PwmEnumsStructs.hpp"
#include "GpioConfigPolicy.hpp" // Importe le concept GpioConfigPolicy
#include <concepts>

namespace WrapperBase {

	template<typename T>
		concept PwmConfigPolicy = requires(T policy) {
			// Vérifie que la politique GPIO imbriquée est valide
			requires GpioConfigPolicy<typename T::GpioPolicy> ;

			// Vérifie l'existence et le type des membres de la configuration PWM
			{ decltype(T::Timer) { } }->std::same_as<PwmTimerInstance> ;
			{ decltype(T::Channel) { } }->std::same_as<PwmTimerChannel> ;
			{ decltype(T::Prescaler) { } }->std::same_as<uint32_t> ;
			{ decltype(T::Period) { } }->std::same_as<uint32_t> ;
			{ decltype(T::Polarity) { } }->std::same_as<PwmPolarity> ;
			{ decltype(T::Mode) { } }->std::same_as<PwmMode> ;
		};

} // namespace WrapperBase
