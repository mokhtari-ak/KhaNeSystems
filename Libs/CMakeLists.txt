add_library(khane_libs INTERFACE)
target_link_libraries(khane_libs INTERFACE 
    common 
    rtos_abstract 
    internal_communication
)
