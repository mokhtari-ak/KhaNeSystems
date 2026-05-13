cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_library(WrapperPolicies INTERFACE AdcConfigPolicy.hpp CanConfigPolicy.hpp DacConfigPolicy.hpp GpioConfigPolicy.hpp I2cConfigPolicy.hpp PwmConfigPolicy.hpp 
    SpiConfigPolicy.hpp UartConfigPolicy.hpp)

target_link_libraries(WrapperPolicies INTERFACE
    WrapperTypes)

target_include_directories(WrapperPolicies INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})