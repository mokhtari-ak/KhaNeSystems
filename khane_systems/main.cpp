#include "FlightController/FlightController.hpp"
#include "Drivers/IMU/MPU6050.hpp"
#include "Drivers/IMU/GpsDriver.hpp"
#include "Modules/SmartPowerSensor.hpp"
#include "Modules/AttitudeEstimatorImpl.hpp"
#include "Modules/Controller.hpp"
#include "Modules/MotorMixer.hpp"
#include "FlightController/Supervisor.hpp"
#include "Modules/FlightLogger.hpp"
#include "Modules/ParameterRegistry.hpp"
#include "Modules/MavlinkTelemetry.hpp"
#include "Modules/NavigationController.hpp"
#include "Modules/MissionExecutor.hpp"

// Mocks simples
class AttitudeEstimatorImpl : public AttitudeEstimator { /* ... */ };
class PidController : public Controller { /* ... */ };
class DshotMixer : public MotorMixer { /* ... */ };
class NavigationControllerImpl : public NavigationController { 
    NavCommands compute(const AttitudeState& s, const Position& p) override { return {}; }
};
class MissionExecutorImpl : public MissionExecutor {
    void add_waypoint(const Position& pos) override {}
    Position get_current_waypoint() const override { return {}; }
    void update(const Position& pos) override {}
};
class StorageMock : public IStorage {
    bool write(const uint8_t* d, size_t l) override { return true; }
    bool sync() override { return true; }
};

int main() {
    SpiBusMock bus;
    Mpu6050Driver gyro(bus);
    Mpu6050Driver accel(bus);
    GpsDriver gps(bus);
    SmartPowerSensor power(bus);
    
    AttitudeEstimatorImpl estimator;
    PidController controller(ParameterRegistry{});
    NavigationControllerImpl nav;
    MissionExecutorImpl mission;
    DshotMixer mixer;
    Supervisor supervisor;
    FlightLogger logger;
    ParameterRegistry registry;
    MavlinkTelemetry telemetry(bus, registry);
    StorageMock storage;

    FlightController<Mpu6050Driver, Mpu6050Driver, GpsDriver> fc(
        gyro, accel, gps, estimator, controller, nav, mission, mixer, supervisor, logger, registry, power
    );

    while (true) {
        fc.update();
        fc.update_slow(storage, telemetry); 
    }
}
