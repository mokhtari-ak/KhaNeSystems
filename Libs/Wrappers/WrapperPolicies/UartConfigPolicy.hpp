#pragma once

#include "UartEnumsStructs.hpp"
#include "GpioConfigPolicy.hpp"
#include <concepts>

namespace WrapperBase {
	template<typename T>
		concept UartConfigPolicy = requires(T policy) {
			requires GpioConfigPolicy<typename T::TxPin>;
			requires GpioConfigPolicy<typename T::RxPin>;
			{ decltype(T::Port) { } }->std::same_as<UartPort>;
			{ decltype(T::BaudRate) {} }->std::same_as<uint32_t>;
			{ decltype(T::WordLength) {} }->std::same_as<UartWordLength>;
			{ decltype(T::StopBits) {} }->std::same_as<UartStopBits>;
			{ decltype(T::Parity) {} }->std::same_as<UartParity>;
			{ decltype(T::Mode) {} }->std::same_as<UartMode>;
			{ decltype(T::HwControl) {} }->std::same_as<UartHwControl>;
			{ decltype(T::Oversampling) {} }->std::same_as<UartOversampling>;
		};
}
