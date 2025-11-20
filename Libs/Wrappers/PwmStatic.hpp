#pragma once

#include "PwmEnumsStructs.hpp"
#include "PwmConfigPolicy.hpp"
#include "IPwmDriver.hpp"
#include "PwmDriver.hpp"

// Inclut les wrappers GPIO que vous avez fournis
#include "GpioStatic.hpp"
#include "GpioDriver.hpp"

using namespace Hal;
using namespace WrapperBase;

namespace Wrapper {

	template<PwmConfigPolicy config, typename Driver = HalPwmDriver, typename GpioDriver = HalGpioDriver>
		class PwmStatic {
		public:
			PwmStatic() = default;

			/**
			 * @brief Initialise la broche GPIO en mode Alternate Function
			 * ET initialise le timer et le canal PWM.
			 */
			void init() {
				// 1. Initialiser la broche GPIO
				// Nous créons une instance temporaire de GpioStatic avec la politique
				// GPIO fournie dans notre configuration PWM.
				GpioStatic<config::GpioPolicy, GpioDriver> gpio_pin;
				gpio_pin.init();

				// 2. Initialiser le driver PWM (Timer + Canal)
				driver.template init<config>();
			}
        
			/**
			 * @brief Démarre la génération du signal PWM sur le canal.
			 */
			void start() { 
				driver.start(config::Timer, config::Channel);
			}
        
			/**
			 * @brief Arrête la génération du signal PWM sur le canal.
			 */
			void stop() {
				driver.stop(config::Timer, config::Channel);
			}
        
			/**
			 * @brief Définit le rapport cyclique via la valeur brute du registre (Pulse).
			 * @param pulse Valeur entre 0 et config::Period.
			 */
			void setDutyCycle(uint32_t pulse) {
				// S'assure que le pulse ne dépasse pas la période max
				if (pulse > config::Period) {
					pulse = config::Period;
				}
				driver.setDutyCycle(config::Timer, config::Channel, pulse);
			}

			/**
			 * @brief Définit le rapport cyclique en pourcentage.
			 * @param percent Valeur entre 0.0f et 100.0f.
			 */
			void setDutyCyclePercent(float percent) {
				if (percent < 0.0f) percent = 0.0f;
				if (percent > 100.0f) percent = 100.0f;
            
				// Calcule la valeur du pulse
				// On ajoute +1 à la période pour le calcul pour un mapping correct 
				// (ex: 100% de 999 (ARR) doit donner 999)
				// C'est plus précis de calculer sur la base de (Period + 1)
				float pulse_f = (percent / 100.0f) * static_cast<float>(config::Period + 1);
            
				// Si la période est 999, 100% donne 999.99... qu'on caste en 999.
				// Si on veut 100% plein, on peut juste mapper 100.0f à config::Period
				if (percent >= 100.0f) {
					setDutyCycle(config::Period);
				}
				else {
					setDutyCycle(static_cast<uint32_t>(pulse_f));
				}
			}
        
			/**
			 * @brief Met à jour la période (ARR) du timer à la volée.
			 * Cela changera la fréquence de TOUS les canaux de ce timer.
			 */
			void setPeriod(uint32_t period) {
				driver.setPeriod(config::Timer, period);
			}

			/**
			 * @brief Met à jour le prescaler du timer à la volée.
			 * Cela changera la fréquence de TOUS les canaux de ce timer.
			 */
			void setPrescaler(uint32_t prescaler) {
				driver.setPrescaler(config::Timer, prescaler);
			}

		private:
			Driver driver;
		};

} //namespace Wrapper