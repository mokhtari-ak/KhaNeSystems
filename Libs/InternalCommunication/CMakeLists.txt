cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_library(InternalCommunication INTERFACE Topic.hpp)

target_link_libraries(InternalCommunication INTERFACE InternalCommPolicies InternalCommLayer)

target_include_directories(InternalCommunication INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
