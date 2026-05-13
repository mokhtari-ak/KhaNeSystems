function(khane_apply_embedded_cpp_policy target_name)
    if(NOT TARGET ${target_name})
        message(FATAL_ERROR "khane_apply_embedded_cpp_policy: target '${target_name}' not found")
    endif()

    # Embedded firmware policy: prioritize deterministic runtime behavior.
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${target_name} PRIVATE
            -fno-exceptions
            -fno-rtti
            -Wall
            -Wextra
            -Wpedantic
            -Wshadow
            -Wconversion
            -Wsign-conversion
        )
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(${target_name} PRIVATE
            /W4
            /EHs-c-
            /GR-
        )
    endif()

    target_compile_definitions(${target_name} PRIVATE
        KHANE_EXCEPTIONS_ENABLED=0
        KHANE_RTTI_ENABLED=0
    )
endfunction()

function(khane_apply_host_test_cpp_policy target_name)
    if(NOT TARGET ${target_name})
        message(FATAL_ERROR "khane_apply_host_test_cpp_policy: target '${target_name}' not found")
    endif()

    # Host tests/simulation: keep diagnostics high, avoid forcing no-rtti/no-exceptions.
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${target_name} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wshadow
        )
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(${target_name} PRIVATE /W4)
    endif()

    target_compile_definitions(${target_name} PRIVATE
        KHANE_TEST_HOOKS=1
        KHANE_SIMULATION=1
    )
endfunction()
