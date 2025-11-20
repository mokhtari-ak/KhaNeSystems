#pragma once

#include "stm32f4xx_hal.h"
#include "PwmEnumsStructs.hpp"
#include "PwmConfigPolicy.hpp"

using namespace WrapperBase;

namespace Hal {
	/*
	 * @brief Interface for PWM driver, bas niveau.
	 **/
	struct IPwmDriver {
        
		/*
		 * @Brief Initialisation du Timer et du canal PWM
		 **/
		template <PwmConfigPolicy config>
			void init() = 0;
        
		/*
		 * @Brief Démarre la génération du signal PWM
		 **/
		virtual void start(PwmTimerInstance timer, PwmTimerChannel channel) = 0;
        
		/*
		 * @Brief Arrête la génération du signal PWM
		 **/
		virtual void stop(PwmTimerInstance timer, PwmTimerChannel channel) = 0;
        
		/*
		 * @Brief Définit la valeur du "pulse" (le rapport cyclique)
		 * @param pulse La valeur du registre CCR (Compare Capture Register).
		 * Doit être entre 0 et la valeur `Period` de la config.
		 **/
		virtual void setDutyCycle(PwmTimerInstance timer, PwmTimerChannel channel, uint32_t pulse) = 0;
        
		/*
		 * @Brief Met à jour la période (valeur ARR)
		 **/
		virtual void setPeriod(PwmTimerInstance timer, uint32_t period) = 0;
        
		/*
		 * @Brief Met à jour le prescaler (valeur PSC)
		 **/
		virtual void setPrescaler(PwmTimerInstance timer, uint32_t prescaler) = 0;
        

		// --- Fonctions de mapping statiques ---
		static TIM_TypeDef* MapTimerInstance(PwmTimerInstance timer);
		static uint32_t MapTimerChannel(PwmTimerChannel channel);
		static uint32_t MapPolarity(PwmPolarity polarity);
		static uint32_t MapMode(PwmMode mode);
		static void EnableClock(PwmTimerInstance timer);

		virtual ~IPwmDriver() = default;
	};

} // namespace Hal