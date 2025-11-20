cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(FreeRtos_App_Custom 
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/FreeRTOS/Source/include
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/FreeRTOS/Source/CMSIS_RTOS_V2
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/FreeRTOS/Source/portable/GCC/ARM_CM4F
    )

set(FreeRTOS_Src
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/FreeRTOS/Source/croutine.c
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/FreeRTOS/Source/event_groups.c
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/FreeRTOS/Source/list.c
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/FreeRTOS/Source/queue.c
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/FreeRTOS/Source/stream_buffer.c
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/FreeRTOS/Source/tasks.c
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/FreeRTOS/Source/timers.c
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/FreeRTOS/Source/portable/MemMang/heap_4.c
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
)

# Create FreeRTOS static library
add_library(FreeRTOS OBJECT)
target_sources(FreeRTOS PRIVATE ${FreeRTOS_Src})
target_include_directories(FreeRTOS PUBLIC ${FreeRtos_App_Custom})
target_link_libraries(FreeRTOS PUBLIC stm32cubemx)

add_library(FreeRtosWrapper INTERFACE)
target_link_libraries(FreeRtosWrapper INTERFACE stm32cubemx FreeRTOS)
target_include_directories(FreeRtosWrapper INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
