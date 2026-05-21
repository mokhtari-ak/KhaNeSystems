#pragma once

#include <concepts>
#include <cstdint>
#include <expected>
#include <span>

/**
 * @brief Erreurs standardisées pour tous les capteurs du système.
 */
enum class SensorError {
    BusError,
    Timeout,
    DataCorrupted,
    NotInitialized
};

/**
 * @brief Concept C++20 définissant l'interface contractuelle d'un capteur.
 * 
 * Ce concept garantit que tout driver de capteur respecte une interface
 * déterministe, sans allocation dynamique et adaptée aux lectures en rafale (FIFO).
 */
template<typename T>
concept Sensor = requires(T s, std::span<typename T::Data> buffer) {
    // 1. Définition du type de données produit par le capteur
    typename T::Data; 

    // 2. Initialisation matérielle
    { s.init() } -> std::same_as<bool>;
    
    // 3. Lecture en rafale sécurisée
    // Retourne le nombre d'échantillons lus, ou une erreur si la lecture échoue.
    // L'utilisation de std::expected force la gestion des erreurs côté appelant.
    { s.read_batch(buffer) } -> std::convertible_to<std::expected<size_t, SensorError>>;
    
    // 4. Diagnostic de santé
    { s.is_healthy() } -> std::same_as<bool>;
    
    // 5. Métadonnées temporelles
    { s.get_last_timestamp_us() } -> std::convertible_to<uint64_t>;
    { s.get_update_rate_hz() } -> std::convertible_to<uint32_t>;
};
