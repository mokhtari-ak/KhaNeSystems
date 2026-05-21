#include <gtest/gtest.h>
#include "../../Modules/MavlinkTelemetry.hpp"
#include "../../Drivers/IBus.hpp"

// Mock pour IBus
class UartBusMock : public IBus {
public:
    std::expected<size_t, BusError> read(uint8_t reg, uint8_t* data, size_t len) override { return 0; }
    std::expected<size_t, BusError> write(uint8_t reg, const uint8_t* data, size_t len) override {
        // Enregistre les données pour vérification
        last_written_data.assign(data, data + len);
        return len;
    }
    std::vector<uint8_t> last_written_data;
};

TEST(MavlinkTelemetryTest, HeartbeatEmitsCorrectly) {
    UartBusMock mock_bus;
    ParameterRegistry registry;
    MavlinkTelemetry telemetry(mock_bus, registry);

    telemetry.send_heartbeat(SystemState::ARMED);

    // Vérifie qu'un message a été écrit sur le bus
    EXPECT_GT(mock_bus.last_written_data.size(), 0);
    
    // MAVLink heartbeat : vérifie le magic byte (FE en v2, FE/FD en v1)
    // Ici on s'assure que le paquet n'est pas vide
    EXPECT_EQ(mock_bus.last_written_data[0], 0xFD); // MAVLink v2
}
