// SpiConfigPolicy.hpp
#pragma once

#include "SpiEnumsStructs.hpp"
#include "GpioConfigPolicy.hpp" // Pour valider les types de Pin
#include <concepts>
#include <type_traits> // Pour std::same_as

namespace WrapperBase {

	template<typename T>
		concept SpiConfigPolicy = requires(T policy) {
			// Vérifie que SCK, MISO, et MOSI sont des GpioStaticConfig valides
			requires GpioConfigPolicy<typename T::SckPin> ;
			requires GpioConfigPolicy<typename T::MisoPin> ;
			requires GpioConfigPolicy<typename T::MosiPin> ;

			// Vérifie que NssPin est SOIT 'void' SOIT un GpioStaticConfig valide
			requires(std::same_as<typename T::NssPin,
			void> ||
			          GpioConfigPolicy<typename T::NssPin>) ;

			  // Vérifie l'existence et le type de tous les paramètres SPI
			  { decltype(T::Port) { } }->std::same_as<Wrapper::SpiPort> ;
			  { decltype(T::Mode) { } }->std::same_as<Wrapper::SpiMode> ;
			  { decltype(T::Direction) { } }->std::same_as<Wrapper::SpiDirection> ;
			  { decltype(T::DataSize) { } }->std::same_as<Wrapper::SpiDataSize> ;
			  { decltype(T::ClockPolarity) { } }->std::same_as<Wrapper::SpiClockPolarity> ;
			  { decltype(T::ClockPhase) { } }->std::same_as<Wrapper::SpiClockPhase> ;
			  { decltype(T::Nss) { } }->std::same_as<Wrapper::SpiNssMode> ;
			  { decltype(T::Prescaler) { } }->std::same_as<Wrapper::SpiBaudRatePrescaler> ;
			  { decltype(T::FirstBit) { } }->std::same_as<Wrapper::SpiFirstBit> ;
		};

} // namespace WrapperBase