#pragma once

#include <vector>
#include <cmath>

namespace khane::navigation {

struct Waypoint {
    float lat, lon, alt;
};

class WaypointNavigator {
public:
    void add_waypoint(Waypoint wp);
    void update(float current_lat, float current_lon, float current_alt);
    
    bool is_mission_complete() const { return current_wp_index_ >= waypoints_.size(); }

private:
    std::vector<Waypoint> waypoints_;
    size_t current_wp_index_ = 0;
};

} // namespace khane::navigation
