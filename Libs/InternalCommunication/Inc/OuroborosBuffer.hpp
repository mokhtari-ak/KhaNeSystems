#pragma once
#include <array>
#include <cstddef>
#include <atomic>
#include "EventBus.hpp"

namespace Communication::EventBus {

    /**
     * @brief Buffer circulaire déterministe avec politique d'écrasement (Ouroboros).
     * @tparam T Type de message (doit satisfaire EventMessage).
     * @tparam Capacity Nombre maximum de messages dans le buffer.
     */
    template<EventMessage T, size_t Capacity>
    class OuroborosBuffer {
    public:
        OuroborosBuffer() noexcept = default;

        /**
         * @brief Ajoute un message au buffer. Écrase le plus ancien si plein.
         */
        void push(const T& message) noexcept;

        /**
         * @brief Récupère le message le plus récent.
         */
        bool pop(T& outMessage) noexcept;

        size_t size() const noexcept { return m_size.load(); }
        bool empty() const noexcept { return size() == 0; }
        size_t capacity() const noexcept { return Capacity; }

    private:
        std::array<T, Capacity> m_data{};
        size_t m_head{0};
        size_t m_tail{0};
        std::atomic<size_t> m_size{0};
        std::atomic<size_t> m_droppedCount{0};
    };

} // namespace Communication::EventBus

#include "OuroborosBuffer.tpp"
