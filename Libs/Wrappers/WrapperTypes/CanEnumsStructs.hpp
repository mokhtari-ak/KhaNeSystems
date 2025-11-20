#pragma once

#include <cstdint>
#include "GpioEnumsStructs.hpp"
#include "GpioStatic.hpp"

namespace WrapperBase {

	/*
	 * @brief Énumération des périphériques CAN disponibles.
	 **/ 
	enum class CanPort {
		CAN_1,
		CAN_2
		// CAN_3 sur certains MCU
	};

	/*
	 * @brief Modes de fonctionnement du CAN. (CAN Operating Mode)
	 **/ 
	enum class CanMode {
		Normal, /*!< Mode normal (transmission/réception) */
		Loopback, /*!< Mode auto-test (pour le debug) */
		Silent, /*!< Mode silencieux (réception seule) */
		SilentLoopback
	};
    
	/*
	 * @brief Filtres FIFO à utiliser.
	 **/ 
	enum class CanRxFifo {
		FIFO_0,
		FIFO_1
	};
    
	/*
	 * @brief Type d'identifiant (ID) d'un message CAN.
	 **/ 
	enum class CanIdType {
		Standard, /*!< 11-bit ID */
		Extended  /*!< 29-bit ID */
	};


	/*
	 * @Brief Segments de propagation: Temps quantique dans le segment de bit 1. (CAN Time Quantum in Bit Segment 1)
	 **/
	enum class TimeQuantaInBitSegment1
	{
		BS1_1,
		BS1_2,
		BS1_3,
		BS1_4,
		BS1_5,
		BS1_6,
		BS1_7,
		BS1_8,
		BS1_9,
		BS1_10,
		BS1_11,
		BS1_12,
		BS1_13,
		BS1_14,
		BS1_15,
	};

	/*
	 * @Brief Phase de propagation: Temps quantique dans le segment de bit 2. (CAN Time Quantum in Bit Segment 2)
	 **/
	enum class TimeQuantaInBitSegment2
	{
		BS2_1,
		BS2_2,
		BS2_3,
		BS2_4,
		BS2_5,
		BS2_6,
		BS2_7,
		BS2_8,
	};

	/*
	 * @Brief Largeur de saut de resynchronisation. (CAN Synchronization Jump Width)
	 **/
	enum class SynchJumpWidth
	{
		SyncJump_1,
		SyncJump_2,
		SyncJump_3,
		SyncJump_4,	
	};
	
	/*
	 * @Brief Mode de filtre CAN (masque ou liste). (CAN Filter Mode)
	 **/
	enum class CanFilterMode
	{
		Mask, /*!< Mode masque */
		List  /*!< Mode liste */
	};

	/*
	 * @Brief Échelle de filtre CAN (16 bits ou 32 bits). (CAN Filter Scale)
	 **/
	enum class CanFilterScale
	{
		Scale_16bit,
		Scale_32bit
	};

	/*
	 * @Brief Fifo du filtre CAN. (CAN Filter FIFO)
	 **/
	enum class CanFilterFifo
	{
		FilterFifo_1,
		FilterFifo_2
	};

	enum class CanResynchJumpWidth
	{
		RJW_1,
		RJW_2,
		RJW_3,
		RJW_4
	};

	/*
	 * @brief Structure de configuration pour le Bit Timing du bus CAN.
	 * Ces valeurs déterminent la vitesse de communication (Baud rate).
	 * Exemple pour 500 kbps avec APB1=42 MHz : Prescaler=6, BS1=13, BS2=2, SJW=1.
	 **/
	template <
		uint32_t Prescaler = 4, /*!< dépend de la fréquence APB1 (ex: 42 MHz sur F407).*/												  // Prescaler de l'horloge
		TimeQuantaInBitSegment1 timeSeg1 = TimeQuantaInBitSegment1::BS1_13,	/*!< Temps Segment 1 (en "time quanta")*/
		TimeQuantaInBitSegment2 timeSeg2 = TimeQuantaInBitSegment2::BS2_2,		/*!< Temps Segment 2 (en "time quanta"). */// 
		CanResynchJumpWidth resyncJumpWidth = CanResynchJumpWidth::RJW_1	/*!< Largeur de saut de resynchronisation. */
		>
	struct CanBitTimingConfig
	{
		static constexpr uint32_t PrescalerValue = Prescaler;
		static constexpr TimeQuantaInBitSegment1 TimeSeg1 = timeSeg1;
		static constexpr TimeQuantaInBitSegment2 TimeSeg2 = timeSeg2;
		static constexpr CanResynchJumpWidth SyncJumpWidth = resyncJumpWidth;
	};

	/*
	 * @brief Structure pour la définition des options CAN
	 **/
	template <
		bool timeTriggeredMode = false,
		bool autoBusOff = false,
		bool autoWakeUp = false,
		bool autoRetransmission = false,
		bool receiveFifoLocked = false,
		bool transmitFifoPriority = false
		>
	struct CanOptionConfig {
		static constexpr bool TimeTriggeredMode = timeTriggeredMode;
		static constexpr bool AutoBusOff = autoBusOff;
		static constexpr bool AutoWakeUp = autoWakeUp;
		static constexpr bool AutoRetransmission = autoRetransmission;
		static constexpr bool ReceiveFifoLocked = receiveFifoLocked;
		static constexpr bool TransmitFifoPriority = transmitFifoPriority;

	};
	
	/*
	 * @Brief Structure de configuration pour les filtres CAN.		
	 **/
	template <
		bool filterActivation = true,
		uint32_t filterIdHigh = 0x00000000,
		uint32_t filterIdLow = 0x00000000,
		uint32_t filterMaskHigh = 0x00000000,
		uint32_t filterMaskLow = 0x00000000,
		CanFilterFifo fifo = CanFilterFifo::FilterFifo_1,
		uint32_t filterBank = 0,
		CanFilterMode filterMode = CanFilterMode::Mask,
		CanFilterScale filterScale = CanFilterScale::Scale_32bit,
		uint32_t slaveStartFilterBank = 14
		>
	struct CanFilterConfig
	{
		static constexpr bool FilterActivation = filterActivation;
		static constexpr bool FilterIdHigh = filterIdHigh;
		static constexpr uint32_t FilterIdLow = filterIdLow;
		static constexpr uint32_t FilterMaskHigh = filterMaskHigh;
		static constexpr uint32_t FilterMaskLow = filterMaskLow;
		static constexpr CanFilterFifo Fifo = fifo;
		static constexpr uint32_t FilterBank = filterBank;
		static constexpr CanFilterMode FilterMode = filterMode;
		static constexpr CanFilterScale FilterScale = filterScale;
		static constexpr uint32_t SlaveStartFilterBank = slaveStartFilterBank;
	};

	/*
	 * @brief Structure de configuration statique pour le périphérique CAN.
	 **/
	template <
		GpioConfigPolicy canRx = GpioStatic<>,
		GpioConfigPolicy canTx = GpioStatic<>,
		CanPort port = CanPort::CAN_1,
		CanMode mode = CanMode::Normal,
		CanBitTimingPolicy TimingConfig = CanBitTimingConfig<>, // Utilise les valeurs par défaut
		CanOptionsPolicy options = CanOptionConfig<>,
		CanFilterPolicy filterOptions = CanFilterConfig<>,
	    bool UseRxInterrupt = true
	>
		struct CanStaticConfig {
			static constexpr CanPort Port = port; /*!< Port CAN */
			static constexpr CanMode Mode = mode; /*!< Mode CAN */
			static constexpr bool UseInterrupt = UseRxInterrupt; /*!< Utiliser Interruption */

			static constexpr uint32_t Prescaler = TimingConfig::PrescalerValue; /*!< Configuration Timing, Prescaler */
			static constexpr uint32_t TimeSeg1 = TimingConfig::TimeSeg1; /*!< Configuration Timing, Time segment 1 */
			static constexpr uint32_t TimeSeg2 = TimingConfig::TimeSeg2; /*!< Configuration Timing, Time segment 2 */
			static constexpr uint32_t ResynchJumpWidth = TimingConfig::ResynchJumpWidth; /*!< Configuration Timing, Resynchronisation */

			static constexpr bool TimeTriggeredMode = options::TimeTriggeredMode; /*!< Configuration des options, Mode de déclenchement */
			static constexpr bool AutoBusOff = options::AutoBusOff; /*!< Configuration des options, AutoBus */
			static constexpr bool AutoWakeUp = options::AutoWakeUp; /*!< Configuration des options, Auto Wake up */
			static constexpr bool AutoRetransmission = options::AutoRetransmission; /*!< Configuration des options, Auto retransmission */
			static constexpr bool ReceiveFifoLocked = options::ReceiveFifoLocked;	/*!< Configuration des options, Mode de réception Fifo Vérouillé */
			static constexpr bool TransmitFifoPriority = options::TransmitFifoPriority; /*!< Configuration des options, Priorité Fifo */

			static constexpr bool FilterActivation = filterOptions::FilterActivation; /*!< Configuration des options, Activer filtration */
			static constexpr bool FilterIdHigh = filterOptions::FilterIdHigh; /*!<  */
			static constexpr uint32_t FilterIdLow = filterOptions::FilterIdLow; /*!<  */
			static constexpr uint32_t FilterMaskHigh = filterOptions::FilterMaskHigh; /*!<  */
			static constexpr uint32_t FilterMaskLow = filterOptions::FilterMaskLow; /*!<  */
			static constexpr CanFilterFifo Fifo = filterOptions::Fifo;	/*!<  */
			static constexpr uint32_t FilterBank = filterOptions::FilterBank; /*!<  */
			static constexpr CanFilterMode FilterMode = filterOptions::FilterMode; /*!<  */
			static constexpr CanFilterScale FilterScale = filterOptions::FilterScale; /*!<  */
			static constexpr uint32_t SlaveStartFilterBank = filterOptions::SlaveStartFilterBank; /*!<  */

			static constexpr GpioConfigPolicy CanRx = canRx; /*!<  */
			static constexpr GpioConfigPolicy CanTx = canTx; /*!<  */
		};

	/*
	 * @brief Structure pour un message CAN à envoyer ou recevoir.
	 **/ 
	struct CanMessage {
		uint32_t id;
		CanIdType idType;
		uint8_t data[8];
		uint8_t dataLength;
	};
} // namespace WrapperBase