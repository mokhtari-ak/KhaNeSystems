#pragma once

#include "IDacDriver.hpp"
#include "stm32f4xx_hal_dac.h"
#include <map>
#include <cassert>

namespace Hal {

	struct HalDacDriver : public IDacDriver {

		// Stocke un handle HAL pour le périphérique DAC
		// (un seul handle gère les deux canaux)
		inline static std::map<DacPort, DAC_HandleTypeDef> dacHandles;
        
		/// <summary>
		/// @brief Initialise le périphérique DAC (une seule fois).
		/// </summary>
		template <DacConfigPolicy config>
			void init_peripheral() {
				enable_clock(config::Port);

				if (dacHandles.find(config::Port) != dacHandles.end()) {
					return; // Déjà initialisé
				}
            
				DAC_HandleTypeDef* pHandle = &dacHandles[config::Port];
				pHandle->Instance = MapPort(config::Port);
            
				HAL_DAC_Init(pHandle);
			}

		/// <summary>
		/// @brief Configure le canal DAC.
		/// </summary>
		template <DacConfigPolicy config>
			void config_channel() {
				DAC_ChannelConfTypeDef sConfig = { 0 };
            
				// Pour l'instant, trigger logiciel uniquement
				sConfig.DAC_Trigger = DAC_TRIGGER_NONE; 
				sConfig.DAC_OutputBuffer = MapOutputBuffer(config::OutputBuffer);
            
				HAL_DAC_ConfigChannel(
				    &dacHandles[config::Port], 
					&sConfig, 
					MapChannel(config::Channel));
            
				// Après la config, on démarre le canal
				start(config::Port, config::Channel);
			}
        
		/// <summary>
		/// @brief Écrit une valeur sur le canal DAC.
		/// </summary>
		void write(DacPort port, DacChannel channel, DacDataAlign align, uint32_t value) override {
			HAL_DAC_SetValue(
			    &dacHandles[port], 
				MapChannel(channel), 
				MapAlign(align), 
				value);
		}

		/// <summary>
		/// @brief Démarre le canal.
		/// </summary>
		void start(DacPort port, DacChannel channel) override {
			HAL_DAC_Start(&dacHandles[port], MapChannel(channel));
		}

		/// <summary>
		/// @brief Arrête le canal.
		/// </summary>
		void stop(DacPort port, DacChannel channel) override {
			HAL_DAC_Stop(&dacHandles[port], MapChannel(channel));
		}


		// --- Implémentation des Mappages ---

		static DAC_TypeDef* MapPort(DacPort port) {
			// F4 a généralement un seul DAC
			return DAC; 
		}

		static uint32_t MapChannel(DacChannel channel) {
			switch (channel) {
			case DacChannel::Channel_1: return DAC_CHANNEL_1;
			case DacChannel::Channel_2: return DAC_CHANNEL_2;
			}
			return DAC_CHANNEL_1;
		}

		static uint32_t MapAlign(DacDataAlign align) {
			switch (align) {
			case DacDataAlign::Align_12b_Right: return DAC_ALIGN_12B_R;
			case DacDataAlign::Align_12b_Left:  return DAC_ALIGN_12B_L;
			case DacDataAlign::Align_8b_Right:  return DAC_ALIGN_8B_R;
			}
			return DAC_ALIGN_12B_R;
		}
        
		static uint32_t MapOutputBuffer(bool enabled) {
			return enabled ? DAC_OUTPUTBUFFER_ENABLE : DAC_OUTPUTBUFFER_DISABLE;
		}

		static void enable_clock(DacPort port) {
			if (is_clock_enabled(port)) return;
			__HAL_RCC_DAC_CLK_ENABLE();
		}
        
		static bool is_clock_enabled(DacPort port) {
			return RCC->APB1ENR & RCC_APB1ENR_DACEN;
		}
	};

} // namespace Hal