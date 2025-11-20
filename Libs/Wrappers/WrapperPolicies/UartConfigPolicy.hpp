#pragma once

#include "UartEnumsStructs.hpp"
#include <concepts>

namespace WrapperBase {
	template<typename T>
		concept UartConfigPolicy = requires(T policy) {
			{ decltype(T::TxPin) {} }->std::same_as<GpioStaticConfig>;
			{ decltype(T::RxPin) {} }->std::same_as<GpioStaticConfig>;
			{ decltype(T::Port) { } }->std::same_as<UartPort>;
			{ decltype(T::BaudRate) {} }->std::same_as<uint32_t>;
			{ decltype(T::WordLength) {} }->std::same_as<UartWordLength>;
			{ decltype(T::StopBits) {} }->std::same_as<UartStopBits>;
			{ decltype(T::Parity) {} }->std::same_as<UartParity>;
			{ decltype(T::Mode) {} }->std::same_as<UartMode>;
			{ decltype(T::HwControl) {} }->std::same_as<UartHwControl>;
			{ decltype(T::UartOversampling) {} }->std::same_as<Oversampling>;
		};
}