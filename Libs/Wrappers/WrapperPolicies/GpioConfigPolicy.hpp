#pragma once

#include "GpioEnumsStructs.hpp"
#include <concepts>
namespace WrapperBase {
	template<typename T>
		concept GpioConfigPolicy = requires(T policy) {
			{ decltype(T::Port) {} }->std::same_as<GpioPort> ;
			{ decltype(T::Pin) {} }->std::same_as<uint8_t> ;
			{ decltype(T::PinMask) {} }->std::same_as<uint16_t> ;
			{ decltype(T::Mode) {} }->std::same_as<GpioPinMode> ;
			{ decltype(T::Pull) {} }->std::same_as<GpioPullMode> ;
			{ decltype(T::Speed) {} }->std::same_as<GpioPinSpeed> ;
			{ decltype(T::Interrupt) {} }->std::same_as<GpioInterruptEdge> ;
			{ decltype(T::GpioAF) {} }->std::same_as<GpioAlternateFunctionType> ;
			{ decltype(T::CanSet) {} }->std::same_as<bool> ;
			{ decltype(T::CanRead) {} }->std::same_as<bool> ;
			{ decltype(T::CanToggle) {} }->std::same_as<bool> ;
		};
}