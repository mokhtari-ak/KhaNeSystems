# --- KHANESYSTEMS LOGIC ---
add_library(KhaneSystemsLogic STATIC)

target_include_directories(KhaneSystemsLogic PUBLIC 
    Inc
    Autopilot/Inc
    FlightController/Inc
    Propulsion/Inc
    SensorFusion/Inc
    ${CMAKE_SOURCE_DIR}/Libs
)

target_sources(KhaneSystemsLogic PRIVATE
    Autopilot/Src/StateMachine.cpp
    # SensorFusion/Src/SensorFusion.cpp # À venir
)

target_link_libraries(KhaneSystemsLogic PUBLIC RtosAbstract)
