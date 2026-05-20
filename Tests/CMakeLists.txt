# --- UNIT TESTS ---
add_executable(test_ouroboros_buffer
    unit/internal_communication/test_ouroboros_buffer.cpp
)

add_executable(test_logger
    unit/common/test_logger.cpp
)

add_executable(test_robustness
    unit/internal_communication/test_robustness.cpp
)

target_include_directories(test_ouroboros_buffer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../libs
)

target_include_directories(test_logger PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../libs
)

target_include_directories(test_robustness PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../libs
)

# Application de la politique de test (si définie)
if(COMMAND khane_apply_host_test_cpp_policy)
    khane_apply_host_test_cpp_policy(test_ouroboros_buffer)
    khane_apply_host_test_cpp_policy(test_logger)
    khane_apply_host_test_cpp_policy(test_robustness)
endif()
