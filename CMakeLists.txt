cmake_minimum_required(VERSION 3.20)
project(KhaNeSystems LANGUAGES C CXX ASM)

# --- Configuration C++20 ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Options Déterministes & Sécurité (Embedded Critical) ---
if(NOT HOST_BUILD)
    add_compile_options(
        -fno-exceptions
        -fno-rtti
        -fno-threadsafe-statics
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
include(cmake/stm32cubemx/CMakeLists.txt)

# --- Dossiers de la bibliothèque (Libs) ---
add_subdirectory(Libs/common)
add_subdirectory(Libs/rtos_abstract)
add_subdirectory(Libs/internal_communication)

# --- Logique Applicative (KhaneSystems) ---
add_subdirectory(KhaneSystems/supervisor)
add_subdirectory(KhaneSystems/sensor_fusion)
add_subdirectory(KhaneSystems/flight_controller)
add_subdirectory(KhaneSystems/propulsion)
add_subdirectory(KhaneSystems/navigation)
add_subdirectory(KhaneSystems/power_monitor)

# --- Tests ---
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(Tests/unit)
endif()

# --- Cible Principale ---
add_executable(${PROJECT_NAME} Core/Src/main.c) # Stub minimal
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        stm32cubemx
        khane_libs
        khane_app
)
