#pragma once

#include "CanEnumsStructs.hpp"
#include <concepts>

namespace WrapperBase {
    
	template<typename T>
		concept CanBitTimingPolicy = requires(T) {
			{ decltype(T::PrescalerValue) { } }->std::same_as<uint32_t> ;
			{ decltype(T::TimeSeg1) { } }->std::same_as<TimeQuantaInBitSegment1>;
			{ decltype(T::TimeSeg2) { } }->std::same_as<TimeQuantaInBitSegment2>;
			{ decltype(T::ResynchJumpWidth) { } }->std::same_as<CanResynchJumpWidth>;
		};
	
	template <typename T>
		concept CanOptionsPolicy = requires(T) { 
			{ decltype(T::TimeTriggeredMode) { } }->std::same_as<bool> ;
			{ decltype(T::AutoBusOff) { } }->std::same_as<bool> ;
			{ decltype(T::AutoWakeUp) { } }->std::same_as<bool> ;
			{ decltype(T::AutoRetransmission) { } }->std::same_as<bool> ;
			{ decltype(T::ReceiveFifoLocked) { } }->std::same_as<bool> ;
			{ decltype(T::TransmitFifoPriority) { } }->std::same_as<bool> ;
		};

	template <typename T>
	concept CanFilterPolicy = requires(T) {
		{
			decltype(T::FilterActivation){}
		} -> std::same_as<bool>;
		{
			decltype(T::FilterIdHigh){}
		} -> std::same_as<uint32_t>;
		{
			decltype(T::FilterIdLow){}
		} -> std::same_as<uint32_t>;
		{
			decltype(T::FilterMaskHigh){}
		} -> std::same_as<uint32_t>;
		{
			decltype(T::FilterMaskLow){}
		} -> std::same_as<uint32_t>;
		{
			decltype(T::Fifo){}
		} -> std::same_as<CanRxFifo>;
		{
			decltype(T::FilterBank){}
		} -> std::same_as<uint32_t>;
		{
			decltype(T::FilterMode){}
		} -> std::same_as<CanFilterMode>;
		{
			decltype(T::FilterScale){}
		} -> std::same_as<CanFilterScale>;
		{
			decltype(T::SlaveStartFilterBank){}
		} -> std::same_as<uint32_t>;
	}
	};

	template<typename T>
		concept CanConfigPolicy = requires(T) {
			{ decltype(T::Port) { } }->std::same_as<CanPort> ;
			{ decltype(T::Mode) { } }->std::same_as<CanMode> ;
			{ decltype(T::UseInterrupt) { } }->std::same_as<bool> ;
			
		};

} // namespace WrapperBase