#pragma once
#include <array>
#include <cstddef>
#include <atomic>
#include <rtos_control.hpp>
#include "event_bus.hpp"

namespace communication::event_bus {

    /**
     * @brief Buffer circulaire déterministe avec politique d'écrasement (Ouroboros).
     * @tparam T Type de message (doit satisfaire event_message).
     * @tparam Capacity Nombre maximum de messages dans le buffer.
     */
    template<event_message T, size_t Capacity>
    class ouroboros_buffer {
    public:
        ouroboros_buffer() noexcept = default;

        /**
         * @brief Ajoute un message au buffer. Écrase le plus ancien si plein.
         */
        void push(const T& message) noexcept;

        /**
         * @brief Récupère le message le plus ancien (FIFO).
         */
        bool pop(T& out_message) noexcept;

        size_t size() const noexcept { return m_size.load(std::memory_order_acquire); }
        bool empty() const noexcept { return size() == 0; }
        size_t capacity() const noexcept { return Capacity; }
        size_t overwrite_drops() const noexcept { return m_overwrite_dropped_count.load(std::memory_order_relaxed); }

    private:
        std::array<T, Capacity> m_data{};
        std::atomic<size_t> m_head{0};
        std::atomic<size_t> m_tail{0};
        std::atomic<size_t> m_size{0};
        std::atomic<size_t> m_overwrite_dropped_count{0};
        rtos::static_mutex m_mutex; // Added for thread-safe push/pop
    };

    /**
     * @brief Abonné avec file d'attente intégrée (Ouroboros).
     * @tparam T Type de message.
     * @tparam Capacity Taille de la file statique.
     */
    template<event_message T, size_t Capacity>
    class buffered_subscriber : public subscriber_node<T> {
    public:
        /**
         * @brief Callback appelé par le Topic lors d'une publication.
         * Stocke le message dans le buffer interne.
         */
        void on_message(const T& message) noexcept override {
            m_buffer.push(message);
        }

        /**
         * @brief Récupère le message le plus ancien de la file (FIFO).
         * @param out_message Destination du message.
         * @return true si un message a été récupéré, false si la file est vide.
         */
        bool receive(T& out_message) noexcept {
            return m_buffer.pop(out_message);
        }

        size_t size() const noexcept { return m_buffer.size(); }
        bool empty() const noexcept { return m_buffer.empty(); }
        size_t overwrite_drops() const noexcept { return m_buffer.overwrite_drops(); }

    private:
        ouroboros_buffer<T, Capacity> m_buffer;
    };

} // namespace communication::event_bus

#include "ouroboros_buffer.tpp"
