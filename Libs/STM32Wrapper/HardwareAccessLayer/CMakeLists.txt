cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_library(HardwareAccessLayer INTERFACE AdcDriver.hpp CanDriver.hpp DacDriver.hpp GpioDriver.hpp I2cDriver.hpp IAdcDriver.hpp ICanDriver.hpp
	IDacDriver.hpp IGpioDriver.hpp II2cDriver.hpp IPwmDriver.hpp ISpiDriver.hpp IUartDriver.hpp PwmDriver.hpp SpiDriver.hpp UartDriver.hpp)

target_link_libraries(HardwareAccessLayer INTERFACE stm32cubemx WrapperPolicies)

target_include_directories(HardwareAccessLayer INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
