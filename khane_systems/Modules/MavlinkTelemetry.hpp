#pragma once

#include "Telemetry.hpp"
#include "ParameterRegistry.hpp"
#include "../Drivers/IBus.hpp"
#include <cstdint>
#include <array>
#include "mavlink/common/mavlink.h"

class MavlinkTelemetry : public Telemetry {
public:
    MavlinkTelemetry(IBus& uart_bus, ParameterRegistry& registry) 
        : uart_bus_(uart_bus), registry_(registry) {}

    void send_heartbeat(SystemState state) override {
        mavlink_message_t msg;
        
        // MAVLink heartbeat structure
        uint8_t system_status = (state == SystemState::ARMED) ? MAV_STATE_ACTIVE : MAV_STATE_STANDBY;
        
        mavlink_msg_heartbeat_pack(
            1,    // System ID
            200,  // Component ID
            &msg, 
            MAV_TYPE_QUADROTOR, 
            MAV_AUTOPILOT_GENERIC, 
            MAV_MODE_FLAG_STABILIZE_ENABLED, 
            0,    // Custom mode
            system_status
        );

        send_message(msg);
    }

    void send_attitude(const AttitudeState& state) override {
        // Implementation will follow
    }

    void update() override {
        // Logic for receiving param set messages
    }

private:
    IBus& uart_bus_;
    ParameterRegistry& registry_;
    std::array<uint8_t, MAVLINK_MAX_PACKET_LEN> buffer_{};

    void send_message(const mavlink_message_t& msg) {
        uint16_t len = mavlink_msg_to_send_buffer(buffer_.data(), &msg);
        uart_bus_.write(0, buffer_.data(), len);
    }
};
