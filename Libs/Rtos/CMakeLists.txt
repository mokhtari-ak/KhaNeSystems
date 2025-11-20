cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_library(Rtos INTERFACE)

if(NOT DEFINED RTOS)
    message(FATAL_ERROR "Aucun Rtos sélectionné.")
endif()

if(RTOS STREQUAL "THREADX")
    target_link_libraries(Rtos INTERFACE stm32cubemx ThreadXWrapper)
elseif(RTOS STREQUAL "FREERTOS")
    target_link_libraries(Rtos INTERFACE stm32cubemx FreeRtosWrapper)
else()
    message(FATAL_ERROR "Rtos inconnu sélectionné.")
endif()

target_include_directories(Rtos INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})