#pragma once

#include "IGpioDriver.hpp"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "GpioEnumsStructs.hpp"
#include "GpioConfigPolicy.hpp"
#include <cassert>
#include <map>
#include <functional>

using namespace WrapperBase;

namespace Hal {
	struct HalGpioDriver : public IGpioDriver {

		static constexpr size_t MaxPins = 16; // EXTI0 à EXTI15

		inline static std::map<int, std::function<void()>> callbacks = { };
		
		template <GpioConfigPolicy T>
		void init() {
			enable_clock(T::Port);
			auto cfg = getHALConfig<T>();
			HAL_GPIO_Init(MapPort(T::Port), &cfg);
		}
		
		template <GpioConfigPolicy T>
		GPIO_InitTypeDef getHALConfig() {
			GPIO_InitTypeDef gpiohalConfig {};
			gpiohalConfig.Pin = T::PinMask;
			gpiohalConfig.Mode = MapPinMode(T::Mode);
			gpiohalConfig.Pull = MapPull(T::Pull);
			gpiohalConfig.Speed = MapSpeed(T::Speed);
			gpiohalConfig.Alternate = MapAlternateFunction(T::GpioAF);
			
			if (T::Interrupt == GpioInterruptEdge::Rising)
				gpiohalConfig.Mode = GPIO_MODE_IT_RISING;
			else if (T::Interrupt == GpioInterruptEdge::Falling)
				gpiohalConfig.Mode = GPIO_MODE_IT_FALLING;
			else if (T::Interrupt == GpioInterruptEdge::RisingFalling)
				gpiohalConfig.Mode = GPIO_MODE_IT_RISING_FALLING;
			
			return gpiohalConfig;
		}
		
		void attach_interrupt(uint8_t pin, std::function<void()> cb) override {
			if (pin < MaxPins) {
				callbacks[pin] = std::move(cb);
			}
		}
		
		static void handle(uint8_t pin) {
			if (pin < MaxPins && callbacks[pin]) {
				callbacks[pin]();
			}
		}
		
		static constexpr GPIO_TypeDef* MapPort(GpioPort port)
		{
			switch (port) {
			case GpioPort::GPIO_A: return GPIOA;
			case GpioPort::GPIO_B: return GPIOB;
			case GpioPort::GPIO_C: return GPIOC;
			case GpioPort::GPIO_D: return GPIOD;
			case GpioPort::GPIO_E: return GPIOE;
			case GpioPort::GPIO_F: return GPIOF;
			case GpioPort::GPIO_G: return GPIOG;
			case GpioPort::GPIO_H: return GPIOH;
			case GpioPort::GPIO_I: return GPIOI;
			}
			return nullptr;
		}
		
		void write(GpioPort port, uint16_t pinMask, GpioPinState state) override {
			HAL_GPIO_WritePin(MapPort(port), pinMask, static_cast<GPIO_PinState>(state)) ;
		}

		GpioPinState read(GpioPort port, uint16_t pinMask) override {
			return (HAL_GPIO_ReadPin(MapPort(port), pinMask) == GPIO_PIN_SET) ? GpioPinState::Set : GpioPinState::Reset;
		}

		void toggle(GpioPort port, uint16_t pinMask) override {
			HAL_GPIO_TogglePin(MapPort(port), pinMask);
		}
		
		static constexpr uint32_t MapPinMode(GpioPinMode mode) {
			switch (mode) {
			case GpioPinMode::Input:             return GPIO_MODE_INPUT;
			case GpioPinMode::Output:            return GPIO_MODE_OUTPUT_PP;
			case GpioPinMode::AlternateFunction: return GPIO_MODE_AF_PP;
			case GpioPinMode::Analog:            return GPIO_MODE_ANALOG;
			}
			return GPIO_MODE_INPUT; // fallback
		}
		
		static constexpr uint32_t MapPull(GpioPullMode pull) {
			switch (pull) {
			case GpioPullMode::None: return GPIO_NOPULL;
			case GpioPullMode::Up:   return GPIO_PULLUP;
			case GpioPullMode::Down: return GPIO_PULLDOWN;
			}
			return GPIO_NOPULL;
		}
		
		static constexpr uint32_t MapSpeed(GpioPinSpeed speed) {
			switch (speed) {
			case GpioPinSpeed::Low:      return GPIO_SPEED_FREQ_LOW;
			case GpioPinSpeed::Medium:   return GPIO_SPEED_FREQ_MEDIUM;
			case GpioPinSpeed::High:     return GPIO_SPEED_FREQ_HIGH;
			case GpioPinSpeed::VeryHigh: return GPIO_SPEED_FREQ_VERY_HIGH;
			}
			return GPIO_SPEED_FREQ_LOW;
		}
		
		static constexpr uint32_t MapInterruptEdge(GpioInterruptEdge edge) {
			switch (edge) {
			case GpioInterruptEdge::None:           return 0;
			case GpioInterruptEdge::Rising:         return GPIO_MODE_IT_RISING;
			case GpioInterruptEdge::Falling:        return GPIO_MODE_IT_FALLING;
			case GpioInterruptEdge::RisingFalling:  return GPIO_MODE_IT_RISING_FALLING;
			}
			return 0; // fallback
		}
		
		static constexpr uint32_t MapAlternateFunction(GpioAlternateFunctionType af) {
			switch (af) {
			case GpioAlternateFunctionType::None: return 0;
			case GpioAlternateFunctionType::AF0_RTC_50Hz: return GPIO_AF0_RTC_50Hz;
			case GpioAlternateFunctionType::AF0_MCO: return GPIO_AF0_MCO;
			case GpioAlternateFunctionType::AF0_TAMPER: return GPIO_AF0_TAMPER;
			case GpioAlternateFunctionType::AF0_SWJ: return GPIO_AF0_SWJ;
			case GpioAlternateFunctionType::AF0_TRACE: return GPIO_AF0_TRACE;
			case GpioAlternateFunctionType::AF1_TIM1: return GPIO_AF1_TIM1;
			case GpioAlternateFunctionType::AF1_TIM2: return GPIO_AF1_TIM2;
			case GpioAlternateFunctionType::AF2_TIM3: return GPIO_AF2_TIM3;
			case GpioAlternateFunctionType::AF2_TIM4: return GPIO_AF2_TIM4;
			case GpioAlternateFunctionType::AF2_TIM5: return GPIO_AF2_TIM5;
			case GpioAlternateFunctionType::AF3_TIM8: return GPIO_AF3_TIM8;
			case GpioAlternateFunctionType::AF3_TIM9: return GPIO_AF3_TIM9;
			case GpioAlternateFunctionType::AF3_TIM10: return GPIO_AF3_TIM10;
			case GpioAlternateFunctionType::AF3_TIM11: return GPIO_AF3_TIM11;
			case GpioAlternateFunctionType::AF4_I2C1: return GPIO_AF4_I2C1;
			case GpioAlternateFunctionType::AF4_I2C2: return GPIO_AF4_I2C2;
			case GpioAlternateFunctionType::AF4_I2C3: return GPIO_AF4_I2C3;
			case GpioAlternateFunctionType::AF5_SPI1: return GPIO_AF5_SPI1;
			case GpioAlternateFunctionType::AF5_SPI2: return GPIO_AF5_SPI2;
			case GpioAlternateFunctionType::AF6_SPI3: return GPIO_AF6_SPI3;
			case GpioAlternateFunctionType::AF7_USART1: return GPIO_AF7_USART1;
			case GpioAlternateFunctionType::AF7_USART2: return GPIO_AF7_USART2;
			case GpioAlternateFunctionType::AF7_USART3: return GPIO_AF7_USART3;
			case GpioAlternateFunctionType::AF8_UART4: return GPIO_AF8_UART4;
			case GpioAlternateFunctionType::AF8_UART5: return GPIO_AF8_UART5;
			case GpioAlternateFunctionType::AF8_USART6: return GPIO_AF8_USART6;
			case GpioAlternateFunctionType::AF9_CAN1: return GPIO_AF9_CAN1;
			case GpioAlternateFunctionType::AF9_CAN2: return GPIO_AF9_CAN2;
			case GpioAlternateFunctionType::AF9_TIM12 : return GPIO_AF9_TIM12;
			case GpioAlternateFunctionType::AF9_TIM13 : return GPIO_AF9_TIM13;
			case GpioAlternateFunctionType::AF9_TIM14 : return GPIO_AF9_TIM14;
			case GpioAlternateFunctionType::AF10_OTG_FS : return GPIO_AF10_OTG_FS;
			case GpioAlternateFunctionType::AF10_OTG_HS : return GPIO_AF10_OTG_HS;
			case GpioAlternateFunctionType::AF11_ETH : return GPIO_AF11_ETH;
			case GpioAlternateFunctionType::AF12_FSMC : return GPIO_AF12_FSMC;
			case GpioAlternateFunctionType::AF12_OTG_HS_FS : return GPIO_AF12_OTG_HS_FS;
			case GpioAlternateFunctionType::AF12_SDIO : return GPIO_AF12_SDIO;
			case GpioAlternateFunctionType::AF13_DCMI : return GPIO_AF13_DCMI;
			case GpioAlternateFunctionType::AF15_EVENTOUT : return GPIO_AF15_EVENTOUT;
			default : return 0;
			}
		}
		
		static inline void enable_clock(GpioPort port) {
			if (is_enabled(port))
				return;
			switch (port) {
			case GpioPort::GPIO_A : __HAL_RCC_GPIOA_CLK_ENABLE(); break;
			case GpioPort::GPIO_B : __HAL_RCC_GPIOB_CLK_ENABLE(); break;
			case GpioPort::GPIO_C : __HAL_RCC_GPIOC_CLK_ENABLE(); break;
			case GpioPort::GPIO_D : __HAL_RCC_GPIOD_CLK_ENABLE(); break;
			case GpioPort::GPIO_E : __HAL_RCC_GPIOE_CLK_ENABLE(); break;
			case GpioPort::GPIO_F : __HAL_RCC_GPIOF_CLK_ENABLE(); break;
			case GpioPort::GPIO_G : __HAL_RCC_GPIOG_CLK_ENABLE(); break;
			case GpioPort::GPIO_H : __HAL_RCC_GPIOH_CLK_ENABLE(); break;
			case GpioPort::GPIO_I : __HAL_RCC_GPIOI_CLK_ENABLE(); break;
			default : assert("Valeur en dehors de la plage");
			}
		}
		
		static inline bool is_enabled(GpioPort port) {
			switch (port) {
			case GpioPort::GPIO_A : return RCC->AHB1ENR & RCC_AHB1ENR_GPIOAEN;
			case GpioPort::GPIO_B : return RCC->AHB1ENR & RCC_AHB1ENR_GPIOBEN;
			case GpioPort::GPIO_C : return RCC->AHB1ENR & RCC_AHB1ENR_GPIOCEN;
			case GpioPort::GPIO_D : return RCC->AHB1ENR & RCC_AHB1ENR_GPIODEN;
			case GpioPort::GPIO_E : return RCC->AHB1ENR & RCC_AHB1ENR_GPIOEEN;
			case GpioPort::GPIO_F : return RCC->AHB1ENR & RCC_AHB1ENR_GPIOFEN;
			case GpioPort::GPIO_G : return RCC->AHB1ENR & RCC_AHB1ENR_GPIOGEN;
			case GpioPort::GPIO_H : return RCC->AHB1ENR & RCC_AHB1ENR_GPIOHEN;
			case GpioPort::GPIO_I : return RCC->AHB1ENR & RCC_AHB1ENR_GPIOIEN;
			}
			assert("Valeur en dehors de la plage");
		}
		
		
		static void EnableIrq(uint16_t pin, uint32_t PreemptPriority, uint32_t SubPriority) {
			IRQn_Type irq;
			// La logique pour déterminer l'IRQ est correcte
			if (pin == 0) irq = EXTI0_IRQn;
			else if (pin == 1) irq = EXTI1_IRQn;
			else if (pin == 2) irq = EXTI2_IRQn;
			else if (pin == 3) irq = EXTI3_IRQn;
			else if (pin == 4) irq = EXTI4_IRQn;
			else if (pin >= 5 && pin <= 9) irq = EXTI9_5_IRQn;
			else irq = EXTI15_10_IRQn;
			
			HAL_NVIC_SetPriority(irq, PreemptPriority, SubPriority);
			HAL_NVIC_EnableIRQ(irq);
		
		}
	};
}