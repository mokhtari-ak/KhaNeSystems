// SpiStatic.hpp
#pragma once

#include <cstdint>
#include <type_traits> // Pour std::conditional_t et std::is_same_v
#include <cstddef>     // Pour std::nullptr_t
#include "SpiEnumsStructs.hpp"
#include "ISpiDriver.hpp"
#include "SpiDriver.hpp"
#include "GpioStatic.hpp" // Crucial : pour initialiser les broches
#include "SpiConfigPolicy.hpp"

using namespace Hal;

namespace Wrapper {

    template<typename PinConfig>
    struct OptionalGpioStatic {
        using Type = Wrapper::GpioStatic<PinConfig>;
    };

    template<>
    struct OptionalGpioStatic<void> {
        using Type = std::nullptr_t;
    };

    template<SpiConfigPolicy config, typename Driver = HalSpiDriver>
    class SpiStatic {
    public:
        SpiStatic()
            : handleIndex(HalSpiDriver::handleEmpty)
            , m_nss_pin()
            {}

        void init() {
            // 1. Initialiser les broches GPIO SCK, MISO, MOSI
            m_sck_pin.init();
            m_miso_pin.init();
            m_mosi_pin.init();

            // 2. Conditonnellement initialiser la broche NSS (si elle n'est pas 'void')
            if constexpr (!std::is_same_v<typename config::NssPin, void>) {
                m_nss_pin.init();
            }
            
            // 3. Initialiser le périphérique SPI
            int8_t handleindex = m_driver.template init<config>();
                
            if (handleindex != HalSpiDriver::handleEmpty) {
                handleIndex = handleindex;
            }
        }

        // --- Fonctions de communication bloquantes ---
        DriverStatus transmit(const uint8_t* data, uint16_t size, uint32_t timeout = HAL_MAX_DELAY) {
            return m_driver.transmit(handleIndex, data, size, timeout);
        }
        DriverStatus receive(uint8_t* data, uint16_t size, uint32_t timeout = HAL_MAX_DELAY) {
            return m_driver.receive(handleIndex, data, size, timeout);
        }
        DriverStatus transmit_receive(const uint8_t* txData, uint8_t* rxData, uint16_t size, uint32_t timeout = HAL_MAX_DELAY) {
            return m_driver.transmit_receive(handleIndex, txData, rxData, size, timeout);
        }

        // --- Fonctions de communication par interruption ---
        DriverStatus transmit_it(const uint8_t* data, uint16_t size) {
            return m_driver.transmit_it(handleIndex, data, size);
        }
        DriverStatus receive_it(uint8_t* data, uint16_t size) {
            return m_driver.receive_it(handleIndex, data, size);
        }
        DriverStatus transmit_receive_it(const uint8_t* txData, uint8_t* rxData, uint16_t size) {
            return m_driver.transmit_receive_it(handleIndex, txData, rxData, size);
        }

        // --- Gestion des Callbacks ---
        void attach_callbacks(std::function<void()> tx_cb,
                              std::function<void()> rx_cb,
                              std::function<void()> txrx_cb,
                              std::function<void()> error_cb) {
            HalSpiDriver::activate_IRQ(config::Port, uint32_t preempt, uint32_t sub);
            HalSpiDriver::attach_callbacks(handleIndex, std::move(tx_cb), std::move(rx_cb), std::move(txrx_cb), std::move(error_cb));
        }

    private:
        Driver m_driver;
        int8_t handleIndex;
        
        // Instanciation des wrappers GpioStatic pour SCK, MISO, MOSI
        Wrapper::GpioStatic<typename config::SckPin> m_sck_pin;
        Wrapper::GpioStatic<typename config::MisoPin> m_miso_pin;
        Wrapper::GpioStatic<typename config::MosiPin> m_mosi_pin;

        // Instanciation conditionnelle du wrapper GpioStatic pour NSS
        // Si config::NssPin est 'void', m_nss_pin sera de type std::nullptr_t
        // Sinon, il sera du type GpioStatic<config::NssPin>
        typename OptionalGpioStatic<typename config::NssPin>::Type m_nss_pin;
    };

} // namespace Wrapper
