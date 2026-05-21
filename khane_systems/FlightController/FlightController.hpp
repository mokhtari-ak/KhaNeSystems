#pragma once

#include "../Drivers/ISensor.hpp"
#include "../Modules/AttitudeEstimator.hpp"
#include "../Modules/Controller.hpp"
#include "../Modules/MotorMixer.hpp"
#include "../Modules/FlightLogger.hpp"
#include "../Modules/ParameterRegistry.hpp"
#include "../Modules/PowerMonitor.hpp"
#include "../Modules/Telemetry.hpp"
#include "../Modules/NavigationController.hpp"
#include "../Modules/MissionExecutor.hpp"
#include "Supervisor.hpp"
#include <array>

/**
 * @brief Orchestrateur principal du système de vol.
 */
template<Sensor GyroT, Sensor AccelT, Sensor GpsT>
class FlightController {
public:
    FlightController(GyroT& gyro, AccelT& accel, GpsT& gps,
                     AttitudeEstimator& estimator, 
                     Controller& controller, 
                     NavigationController& nav_controller,
                     MissionExecutor& mission_executor,
                     MotorMixer& mixer,
                     Supervisor& supervisor,
                     FlightLogger& logger,
                     ParameterRegistry& registry,
                     PowerMonitor& power)
        : gyro_(gyro), accel_(accel), gps_(gps), estimator_(estimator), 
          controller_(controller), nav_controller_(nav_controller), 
          mission_executor_(mission_executor), mixer_(mixer), 
          supervisor_(supervisor), logger_(logger), power_(power) {}

    void update_slow(IStorage& storage, Telemetry& telemetry) {
        logger_.flush(storage);
        power_.update();
        if (power_.get_health() == BatteryHealth::CRITICAL) {
            supervisor_.force_failsafe();
        }
        
        // Navigation & Mission Execution
        auto gps_res = gps_.read_batch(gps_buf_);
        if (gps_res && gps_res.value() > 0) {
            Position current_pos = { gps_buf_[0].latitude, gps_buf_[0].longitude, gps_buf_[0].altitude };
            mission_executor_.update(current_pos);
        }
        
        // Emission télémétrie
        telemetry.send_heartbeat(supervisor_.get_state());
    }

    void update() {
        if (!supervisor_.can_arm_motors()) {
            mixer_.apply_safe_state();
            return;
        }

        auto gyro_res = gyro_.read_batch(gyro_buf_);
        auto accel_res = accel_.read_batch(accel_buf_);

        if (!gyro_res || !accel_res) {
            supervisor_.force_failsafe();
            mixer_.apply_safe_state(); 
            return;
        }

        size_t count = gyro_res.value();
        for (size_t i = 0; i < count; ++i) {
            estimator_.update(gyro_buf_[i], accel_buf_[i]);
        }

        auto state = estimator_.get_state();
        auto commands = controller_.compute(state);

        mixer_.apply(commands);

        logger_.log({
            .timestamp = 0,
            .roll = state.roll,
            .pitch = state.pitch,
            .yaw = state.yaw,
            .throttle = commands.throttle,
            .state = static_cast<uint8_t>(supervisor_.get_state())
        });
    }

private:
    GyroT& gyro_;
    AccelT& accel_;
    GpsT& gps_;
    AttitudeEstimator& estimator_;
    Controller& controller_;
    NavigationController& nav_controller_;
    MissionExecutor& mission_executor_;
    MotorMixer& mixer_;
    Supervisor& supervisor_;
    FlightLogger& logger_;
    PowerMonitor& power_;

    static constexpr size_t BATCH_SIZE = 8;
    typename GyroT::Data gyro_buf_[BATCH_SIZE];
    typename AccelT::Data accel_buf_[BATCH_SIZE];
    typename GpsT::Data gps_buf_[1];
};
