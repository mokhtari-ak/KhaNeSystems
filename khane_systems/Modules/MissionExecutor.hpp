#pragma once

#include <cstdint>

/**
 * @brief Point géographique (Waypoint).
 */
struct Position {
    double latitude;
    double longitude;
    float altitude;
};

/**
 * @brief Interface pour l'exécution de mission (Waypoint).
 */
class MissionExecutor {
public:
    virtual ~MissionExecutor() = default;

    // Ajoute un waypoint à la file d'attente
    virtual void add_waypoint(const Position& pos) = 0;

    // Retourne le waypoint actuel à atteindre
    virtual Position get_current_waypoint() const = 0;

    // Mise à jour de la logique de mission (passage au waypoint suivant, etc.)
    virtual void update(const Position& current_pos) = 0;
};
