cmake_minimum_required(VERSION 3.20)

project(KhaNeSystemsHookTests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/embedded-cpp-policy.cmake)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Modules/eigen)

add_library(khane_hooks INTERFACE)
target_include_directories(khane_hooks INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(khane_hooks INTERFACE KHANE_TEST_HOOKS=1 KHANE_SIMULATION=1)

add_executable(khane_hook_smoke
    Scenarios/HookSmokeScenario.cpp
)

target_link_libraries(khane_hook_smoke PRIVATE khane_hooks)
khane_apply_host_test_cpp_policy(khane_hook_smoke)

add_executable(khane_imu_template_scenario
    Scenarios/ImuDriverTemplateScenario.cpp
)

target_include_directories(khane_imu_template_scenario PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)
target_link_libraries(khane_imu_template_scenario PRIVATE khane_hooks)
khane_apply_host_test_cpp_policy(khane_imu_template_scenario)

add_executable(khane_ekf_fusion_template_scenario
    Scenarios/EkfFusionNodeTemplateScenario.cpp
)
target_include_directories(khane_ekf_fusion_template_scenario PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)
target_link_libraries(khane_ekf_fusion_template_scenario PRIVATE khane_hooks)
khane_apply_host_test_cpp_policy(khane_ekf_fusion_template_scenario)

# --- UNIT TESTS ---
add_executable(test_ouroboros_buffer
    Unit/InternalCommunication/TestOuroborosBuffer.cpp
)
target_include_directories(test_ouroboros_buffer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../Libs
)
khane_apply_host_test_cpp_policy(test_ouroboros_buffer)
