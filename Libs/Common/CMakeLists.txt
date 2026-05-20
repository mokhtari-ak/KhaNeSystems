add_library(common STATIC 
    stub.cpp
    Src/logger.cpp
)
target_include_directories(common PUBLIC . Inc)
target_link_libraries(common PUBLIC internal_communication rtos_abstract)
