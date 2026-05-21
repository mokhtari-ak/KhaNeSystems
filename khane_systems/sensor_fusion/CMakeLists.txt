add_library(sensor_fusion STATIC SensorFusion.cpp)
target_include_directories(sensor_fusion PUBLIC .)
target_link_libraries(sensor_fusion PUBLIC supervisor)
