#include "WaypointNavigator.hpp"

namespace khane::navigation {

void WaypointNavigator::add_waypoint(Waypoint wp) {
    waypoints_.push_back(wp);
}

void WaypointNavigator::update(float current_lat, float current_lon, float current_alt) {
    if (is_mission_complete()) return;

    const auto& target = waypoints_[current_wp_index_];
    float dist = std::sqrt(std::pow(target.lat - current_lat, 2) + 
                           std::pow(target.lon - current_lon, 2));

    if (dist < 0.0001f) { // Threshold for waypoint arrival
        current_wp_index_++;
    }
}

} // namespace khane::navigation
