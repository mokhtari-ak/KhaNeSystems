cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_library(FreeRtos INTERFACE)

target_link_libraries(FreeRtos INTERFACE stm32cubemx)

target_include_directories(FreeRtos INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})