#pragma once

#include "IAdcDriver.hpp"
#include "stm32f4xx_hal_adc.h"
#include <map>
#include <cassert>

namespace Hal {

	struct HalAdcDriver : public IAdcDriver {

		// Stocke un handle HAL pour chaque périphérique ADC (1, 2, 3)
		inline static std::map<AdcPort, ADC_HandleTypeDef> adcHandles;
        
		/// <summary>
		/// @brief Initialise le périphérique ADC (une seule fois par port).
		/// </summary>
		template <AdcConfigPolicy config>
			void init_peripheral() {
				enable_clock(config::Port);

				// Vérifie si l'init a déjà été faite pour ce port
				if (adcHandles.find(config::Port) != adcHandles.end()) {
					return; // Déjà initialisé
				}
            
				ADC_HandleTypeDef* pHandle = &adcHandles[config::Port];
				pHandle->Instance = MapPort(config::Port);

				// Configuration de base (non-continu, trigger logiciel)
				pHandle->Init.Resolution           = MapResolution(config::Resolution);
				pHandle->Init.ScanConvMode         = DISABLE;
				pHandle->Init.ContinuousConvMode   = DISABLE;
				pHandle->Init.DiscontinuousConvMode = DISABLE;
				pHandle->Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
				pHandle->Init.ExternalTrigConv     = ADC_SOFTWARE_START;
				pHandle->Init.DataAlign            = ADC_DATAALIGN_RIGHT;
				pHandle->Init.NbrOfConversion      = 1;
				pHandle->Init.DMAContinuousRequests = DISABLE;
				pHandle->Init.EOCSelection         = ADC_EOC_SINGLE_CONV;
            
				HAL_ADC_Init(pHandle);
			}

		/// <summary>
		/// @brief Configure le canal spécifique.
		/// </summary>
		template <AdcConfigPolicy config>
			void config_channel() {
				ADC_ChannelConfTypeDef sConfig = { 0 };
				sConfig.Channel = MapChannel(config::Channel);
				sConfig.Rank = 1; // Rang 1 (une seule conversion)
				sConfig.SamplingTime = MapSampleTime(config::SampleTime);
            
				HAL_ADC_ConfigChannel(&adcHandles[config::Port], &sConfig);
			}
        
		/// <summary>
		/// @brief Lit une valeur en mode blocant (polling).
		/// </summary>
		uint32_t read(AdcPort port) override {
			ADC_HandleTypeDef* pHandle = &adcHandles[port];
            
			HAL_ADC_Start(pHandle);
			// Attend la fin de la conversion (10ms timeout)
			if (HAL_ADC_PollForConversion(pHandle, 10) == HAL_OK) {
				uint32_t value = HAL_ADC_GetValue(pHandle);
				HAL_ADC_Stop(pHandle);
				return value;
			}
            
			HAL_ADC_Stop(pHandle);
			return 0; // Erreur/Timeout
		}

		// --- Implémentation des Mappages ---

		static ADC_TypeDef* MapPort(AdcPort port) {
			switch (port) {
			case AdcPort::ADC_1: return ADC1;
			case AdcPort::ADC_2: return ADC2;
			case AdcPort::ADC_3: return ADC3;
			}
			return nullptr; // Géré par assert
		}

		static uint32_t MapChannel(AdcChannel channel) {
			switch (channel) {
			case AdcChannel::Channel_0: return ADC_CHANNEL_0;
			case AdcChannel::Channel_1: return ADC_CHANNEL_1;
				// ... (ajoutez tous les canaux)
			case AdcChannel::Channel_18: return ADC_CHANNEL_18;
			case AdcChannel::TempSensor: return ADC_CHANNEL_TEMPSENSOR;
			case AdcChannel::VRefInt:    return ADC_CHANNEL_VREFINT;
			case AdcChannel::VBat:       return ADC_CHANNEL_VBAT;
			}
			return 0;
		}

		static uint32_t MapResolution(AdcResolution res) {
			switch (res) {
			case AdcResolution::Res_12bit: return ADC_RESOLUTION_12B;
			case AdcResolution::Res_10bit: return ADC_RESOLUTION_10B;
			case AdcResolution::Res_8bit:  return ADC_RESOLUTION_8B;
			case AdcResolution::Res_6bit:  return ADC_RESOLUTION_6B;
			}
			return ADC_RESOLUTION_12B;
		}
        
		static uint32_t MapSampleTime(AdcSampleTime time) {
			switch (time) {
			case AdcSampleTime::Cycles_3:   return ADC_SAMPLETIME_3CYCLES;
			case AdcSampleTime::Cycles_15:  return ADC_SAMPLETIME_15CYCLES;
				// ... (ajoutez tous les temps)
			case AdcSampleTime::Cycles_480: return ADC_SAMPLETIME_480CYCLES;
			}
			return ADC_SAMPLETIME_3CYCLES;
		}

		static void enable_clock(AdcPort port) {
			if (is_clock_enabled(port)) return;
            
			switch (port) {
			case AdcPort::ADC_1: __HAL_RCC_ADC1_CLK_ENABLE(); break;
			case AdcPort::ADC_2: __HAL_RCC_ADC2_CLK_ENABLE(); break;
			case AdcPort::ADC_3: __HAL_RCC_ADC3_CLK_ENABLE(); break;
			}
		}
        
		static bool is_clock_enabled(AdcPort port) {
			switch (port) {
			case AdcPort::ADC_1: return RCC->APB2ENR & RCC_APB2ENR_ADC1EN;
			case AdcPort::ADC_2: return RCC->APB2ENR & RCC_APB2ENR_ADC2EN;
			case AdcPort::ADC_3: return RCC->APB2ENR & RCC_APB2ENR_ADC3EN;
			}
			return false;
		}
	};

} // namespace Hal