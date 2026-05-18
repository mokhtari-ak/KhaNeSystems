#pragma once

namespace Communication::EventBus {

    template<EventMessage T, size_t Capacity>
    void OuroborosBuffer<T, Capacity>::push(const T& message) noexcept {
        // Implémentation minimale (vide) pour le cycle RED
        (void)message;
    }

    template<EventMessage T, size_t Capacity>
    bool OuroborosBuffer<T, Capacity>::pop(T& outMessage) noexcept {
        // Implémentation minimale (échec) pour le cycle RED
        (void)outMessage;
        return false;
    }

} // namespace Communication::EventBus
