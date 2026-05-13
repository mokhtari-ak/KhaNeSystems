#pragma once

#include "IDacDriver.hpp"
#include "stm32f4xx_hal_dac.h"
#include <map>
#include <cassert>

namespace Hal {

	struct HalDacDriver : public IDacDriver {

		/**
		* @Brief Stocke un handle HAL pour le périphérique DAC
		* @Note Le handle est initialisé lors de l'appel à init_peripheral() et utilisé pour toutes les opérations DAC.
		* @Note dans un futur proche Utiliser Eigen3 à la place de std::map pour stocker les handles, afin de réduire l'empreinte mémoire et d'améliorer les performances. Eigen3 offre des structures de données optimisées pour les systèmes embarqués, ce qui peut être bénéfique pour la gestion des ressources dans un environnement à contraintes.
		 */ 
		inline static std::map<DacPort, DAC_HandleTypeDef> dacHandles;
        
		/**
		* @Brief Initialise le périphérique DAC (une seule fois).
		 */
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

		/**
		 * @Brief Configure un canal DAC (peut être appelé plusieurs fois pour différents canaux).
		 * @Note Pour l'instant, la configuration est basique (trigger logiciel, buffer activé/désactivé). Vous pouvez étendre cette fonction pour supporter d'autres options de configuration si nécessaire.
		 * @Note Après la configuration, le canal est automatiquement démarré.
		 */
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
        
		/**
		 * @Brief Écrit une valeur sur le canal DAC spécifié.
		 * @Note La valeur doit être alignée selon le paramètre de configuration (12 bits right/left, 8 bits right). Assurez-vous que la valeur fournie respecte les limites d'alignement pour éviter des comportements inattendus.
		 * @Note Cette fonction est non-blocante et utilise la HAL pour écrire la valeur dans le registre de données du DAC. Si vous avez besoin d'une écriture bloquante ou d'une vérification de l'état du DAC avant d'écrire, vous devrez implémenter une logique supplémentaire pour gérer ces cas.
		 * @Note Vous pouvez également ajouter des fonctions pour lire la valeur actuelle du DAC ou pour vérifier l'état du canal (ex: si le DAC est prêt pour une nouvelle écriture) si nécessaire.
		 * @Note Assurez-vous que le canal DAC est correctement configuré et démarré avant d'appeler cette fonction pour éviter des erreurs d'écriture.
		 * @Note En cas d'erreur lors de l'écriture (ex: si le DAC n'est pas prêt), vous pouvez envisager d'ajouter une gestion d'erreur pour informer l'utilisateur ou pour tenter une nouvelle écriture après un délai.
		 * @Note N'oubliez pas que le DAC est un périphérique analogique, donc les valeurs écrites seront converties en tension de sortie. Assurez-vous que les valeurs fournies correspondent à la plage de tension que vous souhaitez obtenir en sortie.
		 * @Note Si vous utilisez plusieurs canaux DAC, assurez-vous de gérer correctement les configurations et les écritures pour chaque canal afin d'éviter des conflits ou des comportements inattendus.
		 * @Note Enfin, n'oubliez pas de tester votre implémentation avec différents scénarios d'écriture pour vous assurer que le DAC fonctionne comme prévu dans toutes les conditions d'utilisation.
		 */
		void write(DacPort port, DacChannel channel, DacDataAlign align, uint32_t value) override {
			HAL_DAC_SetValue(
			    &dacHandles[port], 
				MapChannel(channel), 
				MapAlign(align), 
				value);
		}

		/**
		* @Brief Démarre le canal.
		*/
		void start(DacPort port, DacChannel channel) override {
			HAL_DAC_Start(&dacHandles[port], MapChannel(channel));
		}

		/**
		 * @Brief Arrête le canal.
		 */
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