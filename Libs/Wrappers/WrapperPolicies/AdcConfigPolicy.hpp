#pragma once

#include "AdcEnumsStructs.hpp"
#include <concepts>

namespace WrapperBase {
    
	template<typename T>
		concept AdcConfigPolicy = requires(T) {
			{ decltype(T::Port) { } }->std::same_as<AdcPort> ;
			{ decltype(T::Channel) { } }->std::same_as<AdcChannel> ;
			{ decltype(T::Resolution) { } }->std::same_as<AdcResolution> ;
			{ decltype(T::SampleTime) { } }->std::same_as<AdcSampleTime> ;
			{ decltype(T::CanRead) { } }->std::same_as<bool> ;
		};

} // namespace WrapperBase