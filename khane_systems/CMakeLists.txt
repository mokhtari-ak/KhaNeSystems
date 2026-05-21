add_library(khane_app INTERFACE)

target_link_libraries(khane_app INTERFACE 
    supervisor
    sensor_fusion
    flight_controller
    propulsion
    navigation
    power_monitor
)
