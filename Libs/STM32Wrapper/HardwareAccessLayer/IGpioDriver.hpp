#pragma once

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "GpioEnumsStructs.hpp"
#include "GpioConfigPolicy.hpp"
#include <functional>

using namespace WrapperBase;


namespace Hal 
{
	/*
	 * @brief Interface for GPIO driver, bas niveau.
	 **/
	struct IGpioDriver {
		/*
		 * @Brief Initialisation Gpio
		 * Cette fonction avec template aide à l'initialisation et l'exploitation d'un port GPIO
		 **/
		template <GpioConfigPolicy config>
		void init();
		
		template <GpioConfigPolicy T>
		GPIO_InitTypeDef getHALConfig();
		
		virtual void attach_interrupt(uint8_t pin, std::function<void()> cb) = 0;
		
		/*
		 * @Brief Ecrire un état Gpio
		 **/
		virtual void write(GpioPort, uint16_t, GpioPinState) = 0;
		
		/*
		 * @Brief Lire un état Gpio
		 **/
		virtual GpioPinState read(GpioPort, uint16_t) = 0;
		/*
		 * @Brief Toggle un Gpio
		 **/
		virtual void toggle(GpioPort, uint16_t) = 0;
		
		static inline void enable_clock(GpioPort port);
		static inline bool is_enabled(GpioPort port);

		static constexpr GPIO_TypeDef *MapPort(GpioPort port);
		static constexpr uint32_t MapPinMode(GpioPinMode mode);
		static constexpr uint32_t MapPull(GpioPullMode pull);
		static constexpr uint32_t MapSpeed(GpioPinSpeed speed);
		static constexpr uint32_t MapInterruptEdge(GpioInterruptEdge edge);
		static constexpr uint32_t MapAlternateFunction(GpioAlternateFunctionType af);
		
		static void EnableIrq(uint16_t pin, uint32_t PreemptPriority, uint32_t SubPriority);
		
		virtual ~IGpioDriver() = default;
	};

}
