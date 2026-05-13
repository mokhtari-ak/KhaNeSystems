cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_library(Drivers INTERFACE
    Sensors/Common/DriverStatus.hpp
    Sensors/Imu/ImuTypes.hpp
    Sensors/Imu/IImuDriver.hpp
    Sensors/Imu/ImuDriver.hpp
    Sensors/Gyroscope/GyroTypes.hpp
    Sensors/Gyroscope/IGyroDriver.hpp
    Sensors/Gyroscope/GyroDriver.hpp
    Sensors/AirSpeed/AirSpeedTypes.hpp
    Sensors/AirSpeed/IAirSpeedDriver.hpp
    Sensors/AirSpeed/AirSpeedDriver.hpp
    SensorFusion/ImuDriverInputAdapter.hpp
    SensorFusion/EkfFusionNode.hpp
    SensorFusion/EkfTuning.hpp
)

target_link_libraries(Drivers INTERFACE Wrappers)
target_include_directories(Drivers INTERFACE ${CMAKE_SOURCE_DIR})
