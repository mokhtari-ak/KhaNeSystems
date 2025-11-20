#pragma once

#include "ICanDriver.hpp"
#include "stm32f4xx_hal.h"
#include "CAN/CanEnumsStructs.hpp"
#include "CAN/CanConfigPolicy.hpp"
#include <map>
#include <cassert>
#include <functional>
#include <iostream> // Pour les messages de debug/erreur (peut être retiré en prod)

using namespace WrapperBase;

namespace Hal {

    struct HalCanDriver : public ICanDriver {

        // Stocke un handle HAL par périphérique CAN (CAN_1, CAN_2)
        inline static std::map<CanPort, CAN_HandleTypeDef> canHandles;
        
        // Stocke les callbacks de réception C++ pour chaque FIFO (0 et 1)
        inline static std::map<CanPort, std::map<CanRxFifo, std::function<void(const CanMessage&)>>> rxCallbacks;
        
        // --- Fonctions de Mapping HAL (à implémenter en entier) ---

        static CAN_TypeDef* MapPort(CanPort port) {
            switch (port) {
                case CanPort::CAN_1: return CAN1;
                case CanPort::CAN_2: return CAN2;
            }
            return nullptr;
        }

        static uint32_t MapMode(CanMode mode) {
            switch (mode) {
                case CanMode::Normal:          return CAN_MODE_NORMAL;
                case CanMode::Loopback:        return CAN_MODE_LOOPBACK;
                case CanMode::Silent:          return CAN_MODE_SILENT;
                case CanMode::SilentLoopback:  return CAN_MODE_SILENT_LOOPBACK;
            }
            return CAN_MODE_NORMAL;
        }

		static uint32_t MapTimeSeg1(TimeQuantaInBitSegment1 tseg1) {
            // Mapping des valeurs statiques aux constantes HAL
            switch (tseg1) {
			    case TimeQuantaInBitSegment1::BS1_1: return CAN_BS1_1TQ;
			    case TimeQuantaInBitSegment1::BS1_2: return CAN_BS1_2TQ;
			    case TimeQuantaInBitSegment1::BS1_3: return CAN_BS1_3TQ;
			    case TimeQuantaInBitSegment1::BS1_4: return CAN_BS1_4TQ;
			    case TimeQuantaInBitSegment1::BS1_5: return CAN_BS1_5TQ;
			    case TimeQuantaInBitSegment1::BS1_6: return CAN_BS1_6TQ;
			    case TimeQuantaInBitSegment1::BS1_7: return CAN_BS1_7TQ;
			    case TimeQuantaInBitSegment1::BS1_8: return CAN_BS1_8TQ;
			    case TimeQuantaInBitSegment1::BS1_9: return CAN_BS1_9TQ;
			    case TimeQuantaInBitSegment1::BS1_10: return CAN_BS1_10TQ;
			    case TimeQuantaInBitSegment1::BS1_11: return CAN_BS1_11TQ;
			    case TimeQuantaInBitSegment1::BS1_12: return CAN_BS1_12TQ;
			    case TimeQuantaInBitSegment1::BS1_13: return CAN_BS1_13TQ;
					
				default: return CAN_BS1_13TQ; 
            }
        }

		static uint32_t MapTimeSeg2(TimeQuantaInBitSegment2 tseg2) {
            // Mapping des valeurs statiques aux constantes HAL
            switch (tseg2) {
    			case TimeQuantaInBitSegment2::BS2_1: return CAN_BS2_1TQ; 
    			case TimeQuantaInBitSegment2::BS2_2: return CAN_BS2_2TQ; 
    			case TimeQuantaInBitSegment2::BS2_3: return CAN_BS2_3TQ; 
    			case TimeQuantaInBitSegment2::BS2_4: return CAN_BS2_4TQ; 
    			case TimeQuantaInBitSegment2::BS2_5: return CAN_BS2_5TQ; 
    		    case TimeQuantaInBitSegment2::BS2_6: return CAN_BS2_6TQ; 
			    case TimeQuantaInBitSegment2::BS2_7: return CAN_BS2_7TQ; 
			    case TimeQuantaInBitSegment2::BS2_8: return CAN_BS2_8TQ; 
				
                default: return CAN_BS2_2TQ;
            }
        }

		static uint32_t MapRjw(ResynchJumpWidth rjw) {
            // Mapping des valeurs statiques aux constantes HAL
            switch (rjw) {
			    case CanResynchJumpWidth::RJW_1 : return CAN_SJW_1TQ;
				case CanResynchJumpWidth::RJW_2 : return CAN_SJW_2TQ;
				case CanResynchJumpWidth::RJW_3 : return CAN_SJW_3TQ;
				case CanResynchJumpWidth::RJW_4 : return CAN_SJW_4TQ;

                default: return CAN_SJW_1TQ;
            }
        }
        
        static uint32_t MapIdType(CanIdType type) {
            switch (type) {
                case CanIdType::Standard: return CAN_ID_STD;
                case CanIdType::Extended: return CAN_ID_EXT;
            }
            return CAN_ID_STD;
        }

        static void enable_clock(CanPort port) {
            // Pour le STM32F407, CAN1 et CAN2 sont sur l'APB1
            switch (port) {
                case CanPort::CAN_1: __HAL_RCC_CAN1_CLK_ENABLE(); break;
                case CanPort::CAN_2: __HAL_RCC_CAN2_CLK_ENABLE(); break;
            }
        }

        // --- Implémentation de ICanDriver ---

        /// @brief Initialise le périphérique CAN et le Bit Timing.
        template <CanConfigPolicy config>
        void init_peripheral() {
            enable_clock(config::Port);

            if (canHandles.find(config::Port) != canHandles.end()) {
                return; // Déjà initialisé
            }
            
            CAN_HandleTypeDef* pHandle = &canHandles[config::Port];

            pHandle->Instance = MapPort(config::Port);
            pHandle->Init.Mode = MapMode(config::Mode);
	        
            // Configuration du Bit Timing
            pHandle->Init.Prescaler = config::Prescaler;
            pHandle->Init.TimeSeg1 = MapTimeSeg1(config::TimeSeg1);
            pHandle->Init.TimeSeg2 = MapTimeSeg2(config::TimeSeg2);
			pHandle->Init.SyncJumpWidth = MapRjw(config::ResynchJumpWidth);

            pHandle->Init.TimeTriggeredMode = DISABLE;
            pHandle->Init.AutoBusOff = ENABLE;
            pHandle->Init.AutoWakeUp = DISABLE;
            pHandle->Init.AutoRetransmission = ENABLE;
            pHandle->Init.ReceiveFifoLocked = DISABLE;
            pHandle->Init.TransmitFifoPriority = DISABLE;
            
            if (HAL_CAN_Init(pHandle) != HAL_OK) {
                // Gestion d'erreur
                assert(false && "CAN Init Failed!");
            }

            // --- Configuration des IRQ (si demandée) ---
            if constexpr (config::UseInterrupt) {
                // Vous devez mapper les IRQ du CAN (ex: CAN1_RX0_IRQn) et les activer
                // HAL_NVIC_SetPriority(...);
                // HAL_NVIC_EnableIRQ(...);
            }
        }
        
        /// @brief Configure les filtres du CAN.
        template <CanConfigPolicy config>
        void config_filter() {
            CAN_FilterTypeDef sFilterConfig;

            sFilterConfig.FilterBank = config::FilterConfig.FilterBank;
            sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
            sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
            sFilterConfig.FilterIdHigh = (config::FilterConfig.FilterId << 5) & 0xFFFF; // STM32 nécessite ce décalage pour 11 bits
            sFilterConfig.FilterIdLow = 0x0000;
            sFilterConfig.FilterMaskIdHigh = (config::FilterConfig.FilterMask << 5) & 0xFFFF;
            sFilterConfig.FilterMaskIdLow = 0x0000;
            sFilterConfig.FilterFIFOAssignment = (config::FilterConfig.Fifo == CanRxFifo::FIFO_0) ? CAN_RX_FIFO0 : CAN_RX_FIFO1;
            sFilterConfig.FilterActivation = CAN_FILTER_ENABLE;
            sFilterConfig.SlaveStartFilterBank = 14; // Dépend du MCU (souvent 14 pour les F4)

            if (HAL_CAN_ConfigFilter(&canHandles[config::Port], &sFilterConfig) != HAL_OK) {
                assert(false && "CAN Filter Config Failed!");
            }
        }

        /// @brief Envoie un message sur le bus (non-blocant).
        bool transmit(CanPort port, const CanMessage& message) override {
            CAN_TxHeaderTypeDef txHeader;
            uint32_t txMailbox;

            txHeader.StdId = (message.idType == CanIdType::Standard) ? message.id : 0;
            txHeader.ExtId = (message.idType == CanIdType::Extended) ? message.id : 0;
            txHeader.RTR = CAN_RTR_DATA;
            txHeader.IDE = MapIdType(message.idType);
            txHeader.DLC = message.dataLength;
            txHeader.TransmitGlobalTime = DISABLE;

            if (HAL_CAN_AddTxMessage(&canHandles[port], &txHeader, (uint8_t*)message.data, &txMailbox) != HAL_OK) {
                // Ici, on pourrait logger ou gérer l'erreur de file d'attente pleine
                return false;
            }
            return true;
        }
        
        /// @brief Réception d'un message via la FIFO (pollé/blocant).
        bool receive_polling(CanPort port, CanRxFifo fifo, CanMessage& message) override {
            CAN_RxHeaderTypeDef rxHeader;
            uint8_t rxData[8];
            uint32_t halFifo = (fifo == CanRxFifo::FIFO_0) ? CAN_RX_FIFO0 : CAN_RX_FIFO1;

            if (HAL_CAN_GetRxMessage(&canHandles[port], halFifo, &rxHeader, rxData) != HAL_OK) {
                return false;
            }
            
            // Mappage du header HAL vers la structure C++ CanMessage
            message.idType = (rxHeader.IDE == CAN_ID_STD) ? CanIdType::Standard : CanIdType::Extended;
            message.id = (message.idType == CanIdType::Standard) ? rxHeader.StdId : rxHeader.ExtId;
            message.dataLength = rxHeader.DLC;
            std::copy(rxData, rxData + rxHeader.DLC, message.data);

            return true;
        }
        
        /// @brief Attache un callback à la réception d'un message par interruption.
        void attach_rx_interrupt(CanPort port, CanRxFifo fifo, std::function<void(const CanMessage&)> cb) override {
            rxCallbacks[port][fifo] = cb;
            uint32_t halFifo = (fifo == CanRxFifo::FIFO_0) ? CAN_IT_RX_FIFO0_MSG_PEND : CAN_IT_RX_FIFO1_MSG_PEND;
            
            // Active l'interruption dans le périphérique
            if (HAL_CAN_ActivateNotification(&canHandles[port], halFifo) != HAL_OK) {
                assert(false && "CAN Interrupt Activation Failed!");
            }
        }
        
        // --- Gestionnaire statique d'interruption ---
        
        /// @brief Fonction statique appelée par le callback HAL (C-style).
        static void handle_rx_callback(CAN_HandleTypeDef* hadc, CanRxFifo fifo) {
            CanPort port; 
            if (hadc->Instance == CAN1) port = CanPort::CAN_1;
            else if (hadc->Instance == CAN2) port = CanPort::CAN_2;
            else return; 
            
            // 1. Lire le message
            CanMessage receivedMsg;
            if (HalCanDriver{}.receive_polling(port, fifo, receivedMsg)) {
                
                // 2. Appeler le callback C++ enregistré
                if (rxCallbacks.count(port) && rxCallbacks[port].count(fifo) && rxCallbacks[port][fifo]) {
                    rxCallbacks[port][fifo](receivedMsg);
                } else {
                    // Message reçu, mais pas de callback attaché (pourrait être utile pour du debug)
                }
            }
        }
    };
} // namespace Hal

// --- Routage des Callbacks C (hors du namespace Hal) ---

// Les routines HAL doivent appeler une fonction dans votre driver
// Déclarez ces fonctions dans l'espace global, puis implémentez-les dans un fichier CanIRQHandler.cpp
extern "C" {
    
    // Callback appelé par la HAL quand un message est en attente dans la FIFO 0
    void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hadc) {
        Hal::HalCanDriver::handle_rx_callback(hadc, WrapperBase::CanRxFifo::FIFO_0);
    }
    
    // Callback appelé par la HAL quand un message est en attente dans la FIFO 1
    void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hadc) {
        Hal::HalCanDriver::handle_rx_callback(hadc, WrapperBase::CanRxFifo::FIFO_1);
    }
    
    // Ajoutez ici les ISR (Interrupt Service Routines) CAN1_RX0_IRQHandler, etc.
    // comme vous l'avez fait pour le GPIO.
}