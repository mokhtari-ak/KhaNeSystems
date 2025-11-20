#pragma once

#include "IPwmDriver.hpp"
#include "PwmEnumsStructs.hpp"
#include "PwmConfigPolicy.hpp"
#include "stm32f4xx_hal.h"
#include <map>
#include <cassert>

namespace Hal {

	struct HalPwmDriver : public IPwmDriver {

		// Map statique pour stocker les handles des timers déjà initialisés
		// Clé: Pointeur vers le registre du timer (ex: TIM2)
		// Valeur: Le handle HAL correspondant
		inline static std::map<TIM_TypeDef*, TIM_HandleTypeDef> m_handles;

		template <PwmConfigPolicy T>
			void init() override {
				TIM_TypeDef* instance = MapTimerInstance(T::Timer);
				TIM_HandleTypeDef htim;

				// Vérifie si ce timer a déjà été initialisé
				bool is_new_timer = (m_handles.find(instance) == m_handles.end());

				if (is_new_timer) {
					// Première initialisation pour ce timer
					EnableClock(T::Timer); // Active l'horloge du timer
                
					htim.Instance = instance;
					htim.Init.Prescaler = T::Prescaler;
					htim.Init.Period = T::Period;
					htim.Init.CounterMode = TIM_COUNTERMODE_UP;
					htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
					htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; // Important pour PWM
                
					// Initialise la base de temps du Timer
					if (HAL_TIM_PWM_Init(&htim) != HAL_OK) {
						assert("HAL_TIM_PWM_Init failed");
					}
                
					m_handles[instance] = htim; // Sauvegarde le handle
				}
				else {
					// Timer déjà initialisé, on récupère juste le handle
					htim = m_handles[instance];
					// Note: Idéalement, on pourrait vérifier si T::Prescaler et T::Period
					// correspondent à ceux déjà configurés. Pour l'instant, on suppose
					// que la première configuration est la bonne.
				}

				// Configuration spécifique du canal PWM
				TIM_OC_InitTypeDef oc_config = { };
				oc_config.OCMode = MapMode(T::Mode);
				oc_config.Pulse = 0; // Duty cycle à 0% au démarrage
				oc_config.OCPolarity = MapPolarity(T::Polarity);
				oc_config.OCFastMode = TIM_OCFAST_DISABLE;

				if (HAL_TIM_PWM_ConfigChannel(&htim, &oc_config, MapTimerChannel(T::Channel)) != HAL_OK) {
					assert("HAL_TIM_PWM_ConfigChannel failed");
				}

				// Met à jour le handle dans la map (au cas où HAL l'aurait modifié)
				m_handles[instance] = htim;
			}

		void start(PwmTimerInstance timer, PwmTimerChannel channel) override {
			TIM_HandleTypeDef* htim = &m_handles[MapTimerInstance(timer)];
			HAL_TIM_PWM_Start(htim, MapTimerChannel(channel));
		}

		void stop(PwmTimerInstance timer, PwmTimerChannel channel) override {
			TIM_HandleTypeDef* htim = &m_handles[MapTimerInstance(timer)];
			HAL_TIM_PWM_Stop(htim, MapTimerChannel(channel));
		}

		void setDutyCycle(PwmTimerInstance timer, PwmTimerChannel channel, uint32_t pulse) override {
			TIM_HandleTypeDef* htim = &m_handles[MapTimerInstance(timer)];
			// Utilise la macro HAL pour une mise à jour efficace (évite de reconfigurer tout le canal)
			__HAL_TIM_SET_COMPARE(htim, MapTimerChannel(channel), pulse);
		}

		void setPeriod(PwmTimerInstance timer, uint32_t period) override {
			TIM_HandleTypeDef* htim = &m_handles[MapTimerInstance(timer)];
			__HAL_TIM_SET_AUTORELOAD(htim, period);
		}
        
		void setPrescaler(PwmTimerInstance timer, uint32_t prescaler) override {
			TIM_HandleTypeDef* htim = &m_handles[MapTimerInstance(timer)];
			__HAL_TIM_SET_PRESCALER(htim, prescaler);
		}

		// --- Implémentation des Mappers ---

		static TIM_TypeDef* MapTimerInstance(PwmTimerInstance timer) {
			switch (timer) {
			case PwmTimerInstance::TIM1:  return TIM1;
			case PwmTimerInstance::TIM2:  return TIM2;
			case PwmTimerInstance::TIM3:  return TIM3;
			case PwmTimerInstance::TIM4:  return TIM4;
			case PwmTimerInstance::TIM5:  return TIM5;
			case PwmTimerInstance::TIM8:  return TIM8;
			case PwmTimerInstance::TIM9:  return TIM9;
			case PwmTimerInstance::TIM10: return TIM10;
			case PwmTimerInstance::TIM11: return TIM11;
			case PwmTimerInstance::TIM12: return TIM12;
			case PwmTimerInstance::TIM13: return TIM13;
			case PwmTimerInstance::TIM14: return TIM14;
			}
			assert("Timer instance not supported");
			return nullptr;
		}

		static uint32_t MapTimerChannel(PwmTimerChannel channel) {
			switch (channel) {
			case PwmTimerChannel::Channel1: return TIM_CHANNEL_1;
			case PwmTimerChannel::Channel2: return TIM_CHANNEL_2;
			case PwmTimerChannel::Channel3: return TIM_CHANNEL_3;
			case PwmTimerChannel::Channel4: return TIM_CHANNEL_4;
			}
			return TIM_CHANNEL_ALL; // Fallback
		}

		static uint32_t MapPolarity(PwmPolarity polarity) {
			return (polarity == PwmPolarity::High) ? TIM_OCPOLARITY_HIGH : TIM_OCPOLARITY_LOW;
		}
        
		static uint32_t MapMode(PwmMode mode) {
			return (mode == PwmMode::PWM1) ? TIM_OCMODE_PWM1 : TIM_OCMODE_PWM2;
		}

		static void EnableClock(PwmTimerInstance timer) {
			// Note: TIM1, TIM8-TIM11 sont sur APB2. Les autres sur APB1.
			switch (timer) {
			case PwmTimerInstance::TIM1:  __HAL_RCC_TIM1_CLK_ENABLE(); break;
			case PwmTimerInstance::TIM2:  __HAL_RCC_TIM2_CLK_ENABLE(); break;
			case PwmTimerInstance::TIM3:  __HAL_RCC_TIM3_CLK_ENABLE(); break;
			case PwmTimerInstance::TIM4:  __HAL_RCC_TIM4_CLK_ENABLE(); break;
			case PwmTimerInstance::TIM5:  __HAL_RCC_TIM5_CLK_ENABLE(); break;
			case PwmTimerInstance::TIM8:  __HAL_RCC_TIM8_CLK_ENABLE(); break;
			case PwmTimerInstance::TIM9:  __HAL_RCC_TIM9_CLK_ENABLE(); break;
			case PwmTimerInstance::TIM10: __HAL_RCC_TIM10_CLK_ENABLE(); break;
			case PwmTimerInstance::TIM11: __HAL_RCC_TIM11_CLK_ENABLE(); break;
			case PwmTimerInstance::TIM12: __HAL_RCC_TIM12_CLK_ENABLE(); break;
			case PwmTimerInstance::TIM13: __HAL_RCC_TIM13_CLK_ENABLE(); break;
			case PwmTimerInstance::TIM14: __HAL_RCC_TIM14_CLK_ENABLE(); break;
			default: assert("Timer clock not supported");
			}
		}
	};

} // namespace Hal