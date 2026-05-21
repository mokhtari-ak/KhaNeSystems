#pragma once

#include "IStorage.hpp"
#include <cstdint>
#include <array>
#include <cstring>

/**
 * @brief Structure d'un événement de vol à logger.
 */
struct LogEntry {
    uint32_t timestamp;
    float roll, pitch, yaw;
    float throttle;
    uint8_t state;
};

/**
 * @brief Logger non-bloquant basé sur un buffer circulaire.
 */
class FlightLogger {
public:
    static constexpr size_t BUFFER_SIZE = 1024;

    void log(const LogEntry& entry) {
        size_t next_head = (head_ + 1) % BUFFER_SIZE;
        if (next_head != tail_) {
            buffer_[head_] = entry;
            head_ = next_head;
        }
    }

    /**
     * @brief Vide les données du buffer vers le support de stockage.
     * Cette méthode doit être appelée dans une tâche de priorité inférieure.
     */
    void flush(IStorage& storage) {
        if (head_ == tail_) return; // Rien à écrire

        // On détermine la taille du bloc à écrire (jusqu'à la fin du buffer ou jusqu'à head)
        size_t to_write = 0;
        if (head_ > tail_) {
            to_write = head_ - tail_;
        } else {
            to_write = BUFFER_SIZE - tail_;
        }

        const uint8_t* data_ptr = reinterpret_cast<const uint8_t*>(&buffer_[tail_]);
        if (storage.write(data_ptr, to_write * sizeof(LogEntry))) {
            tail_ = (tail_ + to_write) % BUFFER_SIZE;
        }
    }

private:
    std::array<LogEntry, BUFFER_SIZE> buffer_{};
    size_t head_{0};
    size_t tail_{0};
};
