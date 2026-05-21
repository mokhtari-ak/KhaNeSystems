#pragma once

#include <cstddef>
#include <cstdint>

/**
 * @brief Interface pour le stockage persistant (SD Card, Flash, etc.).
 * Permet au FlightLogger de déverser ses données de manière agnostique.
 */
class IStorage {
public:
    virtual ~IStorage() = default;

    /**
     * @brief Écrit un bloc de données sur le stockage.
     * @param data Pointeur vers les données.
     * @param length Taille des données en octets.
     * @return true si l'écriture a réussi.
     */
    virtual bool write(const uint8_t* data, size_t length) = 0;

    /**
     * @brief S'assure que toutes les données sont physiquement écrites.
     */
    virtual bool sync() = 0;
};
