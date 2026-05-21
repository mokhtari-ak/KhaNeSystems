#pragma once
#include "Common/SensorTypes.hpp"

namespace Drivers {

    /**
     * @brief Driver IMU concret utilisant les wrappers matériels.
     */
    class IMUDriver : public Sensors::SensorInterface<Sensors::IMUData> {
    public:
        IMUDriver() noexcept = default;

        Sensors::IMUData update() noexcept override;
        bool calibrate() noexcept override;

    private:
        // Ici, on utiliserait le wrapper STM32Wrapper/HardwareAccessLayer
        // Ex: SPIWrapper m_spi;
    };

} // namespace Drivers
