#pragma once

#include "GpioEnumsStructs.hpp"
#include <functional>
#include "IGpioDriver.hpp"
#include "GpioDriver.hpp"
#include "GpioConfigPolicy.hpp"

using namespace Hal;
using namespace WrapperBase;

namespace Wrapper {
	

template<GpioConfigPolicy config, typename Driver = HalGpioDriver>
	class GpioStatic {
	public:
		GpioStatic() = default;

		void init() {
			driver.template init<config>();
		}
		
		void constexpr set_high() { 
			if constexpr (config::CanSet) {
				driver.write(config::Port, config::PinMask, GpioPinState::Set);
			}
		}
		
		void constexpr set_low() {
			if constexpr (config::CanSet) {
				driver.write(config::Port, config::PinMask, GpioPinState::Reset);
			}
		}
		
		void constexpr toggle() { 
			if constexpr (config::CanToggle) {
				driver.toggle(config::Port, config::PinMask);
			}
		}
		
		GpioPinState read() {
			if constexpr (config::CanRead) 
				return driver.read(config::Port, config::PinMask);
			else 
				return GpioPinState::Reset;
		}
		
		void attach_interrupt(std::function<void()> callback) {
			if constexpr (config::Interrupt != GpioInterruptEdge::None) {
				driver.attach_interrupt(config::Pin, std::move(callback));
			}
			Driver::EnableIrq(config::Pin, 0,0);
		}
	private:
		Driver driver;
	};
} //namespace