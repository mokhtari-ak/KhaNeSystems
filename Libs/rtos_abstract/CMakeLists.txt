add_library(rtos_abstract STATIC stub.cpp)

target_include_directories(rtos_abstract PUBLIC Inc)

# Configuration du backend via des definitions CMake.
if(HOST_BUILD)
    target_sources(rtos_abstract PRIVATE Src/host_wrapper.cpp)
elseif(RTOS STREQUAL "THREADX")
    target_sources(rtos_abstract PRIVATE Src/threadx_wrapper.cpp)
    target_compile_definitions(rtos_abstract PUBLIC USE_THREADX_BACKEND)
elseif(RTOS STREQUAL "FREERTOS")
    target_sources(rtos_abstract PRIVATE Src/freertos_wrapper.cpp)
    target_compile_definitions(rtos_abstract PUBLIC USE_FREERTOS_BACKEND)
endif()
