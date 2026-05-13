#pragma once

#include "GpioEnumsStructs.hpp"
#include <functional>
#include "IGpioDriver.hpp"
#include "GpioDriver.hpp"
#include "GpioConfigPolicy.hpp"

namespace Wrapper {
	

template<WrapperBase::GpioConfigPolicy config, typename Driver = Hal::HalGpioDriver>
	class GpioStatic {
	public:
		GpioStatic() = default;

		void init() {
			int8_t idx = driver.template init<config>();
			if (idx == HalUartDriver::handleEmpty)
				return InitResult::HalError;
			handleIndex = idx;
			return InitResult::Ok;
		}
		
		void set_high() { 
			if constexpr (config::CanSet) {
				driver.write(config::Port, config::PinMask, WrapperBase::GpioPinState::Set);
			}
		}
		
		void set_low() {
			if constexpr (config::CanSet) {
				driver.write(config::Port, config::PinMask, WrapperBase::GpioPinState::Reset);
			}
		}
		
		void toggle() { 
			if constexpr (config::CanToggle) {
				driver.toggle(config::Port, config::PinMask);
			}
		}
		
		WrapperBase::GpioPinState read() {
			static_assert(config::CanRead, "GPIO pin is not configured as readable");
			return driver.read(config::Port, config::PinMask);
		}
		
		void attach_interrupt(std::function<void()> callback) {
			if constexpr (config::Interrupt != WrapperBase::GpioInterruptEdge::None) {
				driver.attach_interrupt(config::Pin, std::move(callback));
				Driver::EnableIrq(config::Pin, 0, 0);
			}
		}
	private:
		Driver driver;
	};
} //namespace
