#pragma once
#include <concepts>
#include <type_traits>
#include <cstdint>

namespace Communication::EventBus {

    /**
     * @brief Concept définissant ce qu'est un message valide pour le bus.
     * Un message doit être trivialement copiable pour garantir le déterminisme
     * et permettre des optimisations de transfert (DMA, zero-copy).
     */
    template<typename T>
    concept EventMessage = std::is_trivially_copyable_v<T> && std::is_standard_layout_v<T>;

    /**
     * @brief Interface de base pour un abonné.
     */
    template<EventMessage T>
    class SubscriberInterface {
    public:
        virtual ~SubscriberInterface() = default;
        virtual void onMessage(const T& message) noexcept = 0;
    };

    /**
     * @brief Interface de base pour un éditeur.
     */
    template<EventMessage T>
    class PublisherInterface {
    public:
        virtual ~PublisherInterface() = default;
        virtual void publish(const T& message) noexcept = 0;
    };

} // namespace Communication::EventBus
