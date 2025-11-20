cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_library(Wrappers INTERFACE)

target_link_libraries(Wrappers INTERFACE WrapperTypes WrapperPolicies HardwareAccessLayer)

target_include_directories(Wrappers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
