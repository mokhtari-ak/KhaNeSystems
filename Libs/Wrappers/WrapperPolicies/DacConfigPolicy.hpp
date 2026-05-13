#pragma once

#include "DacEnumsStructs.hpp"
#include <concepts>

namespace WrapperBase {
    
	template<typename T>
		concept DacConfigPolicy = requires(T) {
			{ decltype(T::Port) { } }->std::same_as<DacPort> ;
			{ decltype(T::Channel) { } }->std::same_as<DacChannel> ;
			{ decltype(T::Align) { } }->std::same_as<DacDataAlign> ;
			{ decltype(T::OutputBuffer) { } }->std::same_as<bool> ;
			{ decltype(T::CanSet) { } }->std::same_as<bool> ;
		};

} // namespace WrapperBase