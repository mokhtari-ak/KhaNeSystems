cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_library(WrapperTypes INTERFACE AdcEnumsStructs.hpp CanEnumsStructs.hpp DacEnumsStructs.hpp GpioEnumsStructs.hpp I2cEnumsStructs.hpp 
	PwmEnumsStructs.hpp SpiEnumsStructs.hpp UartEnumsStructs.hpp)

target_include_directories(WrapperTypes INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})