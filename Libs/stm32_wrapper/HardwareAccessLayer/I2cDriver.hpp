#pragma once

#include <vector>
#include <cstdint>
#include <map>
#include <functional>
#include "II2cDriver.hpp"
#include "stm32f4xx_hal.h"
#include "I2cConfigPolicy.hpp"

using namespace WrapperBase;

namespace Hal {

	struct HalI2cDriver : public Hal::II2cDriver {

		inline static constexpr int8_t handleEmpty = -1;
		/**
		* @Brief Vecteur pour stocker les gestionnaires I2C.
		* @Note Chaque gestionnaire correspond à une instance I2C (ex: I2C1, I2C2, etc.) et contient la configuration et l'état du périphérique I2C correspondant.
		* @Note Les gestionnaires sont initialisés lors de l'appel à init() et utilisés pour toutes les opérations I2C.
		 * @Note L'utilisation d'un vecteur permet de facilement étendre le driver à plusieurs périphériques I2C si nécessaire, sans devoir dupliquer le code pour chaque instance. Cependant, assurez-vous de gérer correctement les indices du vecteur pour éviter des accès hors limites.
		 * @Note une modification est necessaire utiliser Eigen3 à la place de std::vector pour stocker les handles, afin de réduire l'empreinte mémoire et d'améliorer les performances. Eigen3 offre des structures de données optimisées pour les systèmes embarqués, ce qui peut être bénéfique pour la gestion des ressources dans un environnement à contraintes.
		 * @Note Assurez-vous que les gestionnaires stockés dans ce vecteur sont correctement initialisés et configurés avant de les utiliser pour des opérations I2C. Vous pouvez également envisager d'ajouter une gestion d'erreur pour les cas où un gestionnaire n'est pas trouvé pour une instance I2C donnée ou pour les cas où une opération I2C échoue en raison d'une configuration incorrecte ou d'un problème matériel.
		 * @Note Enfin, n'oubliez pas de tester votre implémentation avec différents scénarios d'utilisation de l'I2C pour vous assurer que les gestionnaires fonctionnent correctement et que le comportement du système est conforme à vos attentes dans toutes les conditions d'utilisation.
		 * @Note Si vous prévoyez d'utiliser plusieurs instances I2C, assurez-vous de gérer correctement les ressources et les conflits potentiels entre les différentes instances, notamment en ce qui concerne les interruptions, les adresses des périphériques, etc. Vous pouvez également envisager d'ajouter des fonctionnalités supplémentaires pour faciliter la gestion de plusieurs instances I2C, comme des fonctions pour récupérer un gestionnaire par instance ou pour vérifier l'état d'une instance avant de l'utiliser.
		 * @Note N'oubliez pas que l'I2C est un protocole de communication qui peut être sensible aux problèmes de timing et de signal, donc assurez-vous de tester votre implémentation dans des conditions réelles pour vous assurer que les performances et la fiabilité sont au rendez-vous.
		 * @Note Remplacer map par Eigen3 dans un futur proche pour réduire l'empreinte mémoire et améliorer les performances. Eigen3 offre des structures de données optimisées pour les systèmes embarqués, ce qui peut être bénéfique pour la gestion des ressources dans un environnement à contraintes.
		*/
		inline static std::vector<I2C_HandleTypeDef> i2cHandles = { };
        
		// Maps pour les callbacks, indexés par l'instance I2C (I2C1, I2C2...)
		inline static std::map<I2C_TypeDef*, std::function<void()>> tx_complete_callbacks = { };
		inline static std::map<I2C_TypeDef*, std::function<void()>> rx_complete_callbacks = { };
		inline static std::map<I2C_TypeDef*, std::function<void()>> error_callbacks = { };
		// Note : I2C a aussi des callbacks pour MemTx/MemRx, vous pouvez les ajouter si besoin
		/**
		* @Brief Initialise l'I2C.
		* @Note Cette fonction initialise l'I2C en fonction de la configuration spécifiée.
		*/
		template<I2cConfigPolicy T>
			int8_t init() {
				enable_clock(T::Port);

				I2C_HandleTypeDef m_handle = { };
				m_handle.Instance = MapPort(T::Port);
				m_handle.Init = getHALConfig<T>();

				HAL_StatusTypeDef status = HAL_I2C_Init(&m_handle);

				if (status == HAL_OK) {
					i2cHandles.push_back(m_handle);
					return i2cHandles.size() - 1;
				}
				return handleEmpty;
			}

		// --- Implémentation des fonctions de l'interface ---

		DriverStatus master_transmit(int8_t handleIndex, uint16_t devAddress, const uint8_t* data, uint16_t size, uint32_t timeout) override {
			auto result = HAL_I2C_Master_Transmit(&i2cHandles[handleIndex], devAddress, const_cast<uint8_t*>(data), size, timeout);
			return From_hal(result);
		}
		DriverStatus master_receive(int8_t handleIndex, uint16_t devAddress, uint8_t* data, uint16_t size, uint32_t timeout) override {
			auto result = HAL_I2C_Master_Receive(&i2cHandles[handleIndex], devAddress, data, size, timeout);
			return From_hal(result);
		}
		DriverStatus mem_write(int8_t handleIndex, uint16_t devAddress, uint16_t memAddress, uint16_t memAddSize, const uint8_t* data, uint16_t size, uint32_t timeout) override {
			auto result = HAL_I2C_Mem_Write(&i2cHandles[handleIndex], devAddress, memAddress, memAddSize, const_cast<uint8_t*>(data), size, timeout);
			return From_hal(result);
		}
		DriverStatus mem_read(int8_t handleIndex, uint16_t devAddress, uint16_t memAddress, uint16_t memAddSize, uint8_t* data, uint16_t size, uint32_t timeout) override {
			auto result = HAL_I2C_Mem_Read(&i2cHandles[handleIndex], devAddress, memAddress, memAddSize, data, size, timeout);
			return From_hal(result);
		}

		DriverStatus master_transmit_it(int8_t handleIndex, uint16_t devAddress, const uint8_t* data, uint16_t size) override {
			return From_hal(HAL_I2C_Master_Transmit_IT(&i2cHandles[handleIndex], devAddress, const_cast<uint8_t*>(data), size));
		}
		DriverStatus master_receive_it(int8_t handleIndex, uint16_t devAddress, uint8_t* data, uint16_t size) override {
			return From_hal(HAL_I2C_Master_Receive_IT(&i2cHandles[handleIndex], devAddress, data, size));
		}
		DriverStatus mem_write_it(int8_t handleIndex, uint16_t devAddress, uint16_t memAddress, uint16_t memAddSize, const uint8_t* data, uint16_t size) override {
			return From_hal(HAL_I2C_Mem_Write_IT(&i2cHandles[handleIndex], devAddress, memAddress, memAddSize, const_cast<uint8_t*>(data), size));
		}
		DriverStatus mem_read_it(int8_t handleIndex, uint16_t devAddress, uint16_t memAddress, uint16_t memAddSize, uint8_t* data, uint16_t size) override {
			return From_hal(HAL_I2C_Mem_Read_IT(&i2cHandles[handleIndex], devAddress, memAddress, memAddSize, data, size));
		}

		// --- Fonctions statiques de gestion (callbacks, IRQ, clock) ---

		static void attach_callbacks(int8_t handle_index, std::function<void()> tx_cb, std::function<void()> rx_cb, std::function<void()> error_cb) {
			I2C_TypeDef* instance = i2cHandles[handle_index].Instance;
			if (tx_cb) tx_complete_callbacks[instance] = std::move(tx_cb);
			if (rx_cb) rx_complete_callbacks[instance] = std::move(rx_cb);
			if (error_cb) error_callbacks[instance] = std::move(error_cb);
		}

		// Fonctions à appeler depuis les callbacks globaux HAL (ex: stm32f4xx_it.c)
		static void handle_tx_complete(I2C_HandleTypeDef *hi2c) {
			if (tx_complete_callbacks.count(hi2c->Instance)) {
				tx_complete_callbacks[hi2c->Instance]();
			}
		}
		static void handle_rx_complete(I2C_HandleTypeDef *hi2c) {
			if (rx_complete_callbacks.count(hi2c->Instance)) {
				rx_complete_callbacks[hi2c->Instance]();
			}
		}
		static void handle_error(I2C_HandleTypeDef *hi2c) {
			if (error_callbacks.count(hi2c->Instance)) {
				error_callbacks[hi2c->Instance]();
			}
		}

		static void activate_IRQ(I2cPort port, uint32_t preempt, uint32_t sub) {
			IRQn_Type ev_irq, er_irq;
			switch (port) {
			case I2cPort::I2C_1: ev_irq = I2C1_EV_IRQn; er_irq = I2C1_ER_IRQn; break;
			case I2cPort::I2C_2: ev_irq = I2C2_EV_IRQn; er_irq = I2C2_ER_IRQn; break;
			case I2cPort::I2C_3: ev_irq = I2C3_EV_IRQn; er_irq = I2C3_ER_IRQn; break;
			}
			HAL_NVIC_SetPriority(ev_irq, preempt, sub);
			HAL_NVIC_EnableIRQ(ev_irq);
			HAL_NVIC_SetPriority(er_irq, preempt, sub);
			HAL_NVIC_EnableIRQ(er_irq);
		}

		static void enable_clock(I2cPort port) {
			if (is_enabled(port)) return;
			switch (port) {
			case I2cPort::I2C_1: __HAL_RCC_I2C1_CLK_ENABLE(); break;
			case I2cPort::I2C_2: __HAL_RCC_I2C2_CLK_ENABLE(); break;
			case I2cPort::I2C_3: __HAL_RCC_I2C3_CLK_ENABLE(); break;
			}
		}

		static bool is_enabled(I2cPort port) {
			switch (port) {
			case I2cPort::I2C_1: return RCC->APB1ENR & RCC_APB1ENR_I2C1EN;
			case I2cPort::I2C_2: return RCC->APB1ENR & RCC_APB1ENR_I2C2EN;
			case I2cPort::I2C_3: return RCC->APB1ENR & RCC_APB1ENR_I2C3EN;
			}
			return false;
		}

		// --- Fonctions de mappage statiques ---

		static constexpr I2C_TypeDef* MapPort(I2cPort port) {
			switch (port) {
			case I2cPort::I2C_1: return I2C1;
			case I2cPort::I2C_2: return I2C2;
			case I2cPort::I2C_3: return I2C3;
			}
			return nullptr;
		}

		static constexpr uint32_t MapAddressingMode(I2cAddressingMode mode) {
			return (mode == I2cAddressingMode::Mode10Bit) ? I2C_ADDRESSINGMODE_10BIT : I2C_ADDRESSINGMODE_7BIT;
		}

		static constexpr uint32_t MapDutyCycle(I2cDutyCycle cycle) {
			return (cycle == I2cDutyCycle::Cycle16_9) ? I2C_DUTYCYCLE_16_9 : I2C_DUTYCYCLE_2;
		}

		static DriverStatus From_hal(HAL_StatusTypeDef s) {
			switch(s) {
				case HAL_OK:      return DriverStatus::Ok;
				case HAL_BUSY:    return DriverStatus::Busy;
				case HAL_TIMEOUT: return DriverStatus::Timeout;
				default:          return DriverStatus::Error;
			}
		}

		template<I2cConfigPolicy config>
			static constexpr I2C_InitTypeDef getHALConfig() {
				I2C_InitTypeDef i2cConfig {}
				;
				i2cConfig.ClockSpeed = config::ClockSpeed;
				i2cConfig.DutyCycle = MapDutyCycle(config::DutyCycle);
				i2cConfig.OwnAddress1 = config::OwnAddress1;
				i2cConfig.AddressingMode = MapAddressingMode(config::AddressingMode);
				i2cConfig.DualAddressMode = config::DualAddressMode ? I2C_DUALADDRESS_ENABLE : I2C_DUALADDRESS_DISABLE;
				i2cConfig.OwnAddress2 = config::OwnAddress2;
				i2cConfig.GeneralCallMode = config::GeneralCallMode ? I2C_GENERALCALL_ENABLE : I2C_GENERALCALL_DISABLE;
				i2cConfig.NoStretchMode = config::NoStretchMode ? I2C_NOSTRETCH_ENABLE : I2C_NOSTRETCH_DISABLE;
				return i2cConfig;
			}
	};

} // namespace Hal
