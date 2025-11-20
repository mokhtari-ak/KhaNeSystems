cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_library(RtosAbstract INTERFACE)

target_link_libraries(RtosAbstract INTERFACE stm32cubemx)

target_include_directories(RtosAbstract INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})