#pragma once

#include "I2cEnumsStructs.hpp"
#include "GpioConfigPolicy.hpp" // Pour valider les types de Pin
#include <concepts>

namespace WrapperBase {
	template<typename T>
		concept I2cConfigPolicy = requires(T policy) {
			// Vérifie que SclPin et SdaPin sont des GpioStaticConfig valides
			requires GpioConfigPolicy<typename T::SclPin> ;
			requires GpioConfigPolicy<typename T::SdaPin> ;

			{ decltype(T::Port) { } }->std::same_as<Wrapper::I2cPort> ;
			{ decltype(T::ClockSpeed) { } }->std::same_as<uint32_t> ;
			{ decltype(T::AddressingMode) { } }->std::same_as<Wrapper::I2cAddressingMode> ;
			{ decltype(T::DutyCycle) { } }->std::same_as<Wrapper::I2cDutyCycle> ;
			{ decltype(T::OwnAddress1) { } }->std::same_as<uint32_t> ;
			{ decltype(T::GeneralCallMode) { } }->std::same_as<bool> ;
			{ decltype(T::NoStretchMode) { } }->std::same_as<bool> ;
			{ decltype(T::OwnAddress2) { } }->std::same_as<uint32_t> ;
			{ decltype(T::DualAddressMode) { } }->std::same_as<bool> ;
		};
} // namespace WrapperBase
