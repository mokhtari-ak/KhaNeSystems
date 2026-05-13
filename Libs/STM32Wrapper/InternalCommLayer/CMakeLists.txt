cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_library(InternalCommLayer INTERFACE ThreadXMessageQueue.hpp IMessageQueue.hpp)

target_link_libraries(InternalCommLayer INTERFACE InternalCommPolicies)

if(DEFINED RTOS)
    if(RTOS STREQUAL "THREADX")
        target_compile_definitions(InternalCommLayer INTERFACE USE_THREADX)
    elseif(RTOS STREQUAL "FREERTOS")
        target_compile_definitions(InternalCommLayer INTERFACE USE_FREERTOS)
    endif()
endif()

target_include_directories(InternalCommLayer INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
