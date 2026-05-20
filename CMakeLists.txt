cmake_minimum_required(VERSION 3.20)
project(KhaNeSystems LANGUAGES C CXX ASM)

option(HOST_BUILD "Build host-side tests/simulation instead of STM32 firmware" OFF)

# --- Configuration C++20 ---
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/embedded-cpp-policy.cmake)

# --- Options Déterministes & Sécurité (Embedded Critical) ---
if(NOT HOST_BUILD)
    add_compile_options(
        $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
        $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
        $<$<COMPILE_LANGUAGE:CXX>:-fno-threadsafe-statics>
        -ffunction-sections
        -fdata-sections
        -Wall
        -Wextra
        -Werror
        -Wpedantic
    )
    add_link_options(-Wl,--gc-sections)
endif()

# --- Inclusion de la HAL (STM32CubeMX) ---
if(NOT HOST_BUILD)
    add_executable(${PROJECT_NAME} Core/Src/main.cpp)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:-Wno-error=missing-field-initializers>
    )
    include(cmake/stm32cubemx/CMakeLists.txt)
else()
    # Dummy interface for host tests
    add_library(stm32cubemx INTERFACE)
endif()

# --- Dossiers de la bibliothèque (libs) ---
add_subdirectory(libs/rtos_abstract)
add_subdirectory(libs/internal_communication)
add_subdirectory(libs/common)

# --- Logique Applicative (khane_systems) ---
add_subdirectory(khane_systems/supervisor)
add_subdirectory(khane_systems/sensor_fusion)
add_subdirectory(khane_systems/flight_controller)
add_subdirectory(khane_systems/propulsion)
add_subdirectory(khane_systems/navigation)
add_subdirectory(khane_systems/power_monitor)

# --- Bibliothèques Externes (Modules) ---
add_subdirectory(modules/eigen)
add_subdirectory(modules/KalmanFilterLibrary)
if(NOT TARGET KalmanFilter)
    add_library(KalmanFilter INTERFACE)
    target_include_directories(KalmanFilter INTERFACE
        ${CMAKE_SOURCE_DIR}/modules/KalmanFilterLibrary/include
    )
    target_link_libraries(KalmanFilter INTERFACE Eigen3::Eigen)
endif()

# --- Tests ---
if(HOST_BUILD AND BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

# --- Cible Principale (Firmware) ---
if(NOT HOST_BUILD)
    target_link_libraries(${PROJECT_NAME} 
        PRIVATE 
            stm32cubemx
            common
            rtos_abstract
            internal_communication
            supervisor
            sensor_fusion
            flight_controller
            propulsion
            navigation
            power_monitor
            Eigen3::Eigen
            KalmanFilter
    )
endif()
