#pragma once
#include <concepts>
#include <type_traits>
#include <cstdint>
#include <atomic>
#include <rtos_control.hpp>

namespace communication::event_bus {

    /**
     * @brief Priorités des messages pour la gestion de l'encombrement du bus.
     */
    enum class message_priority : uint8_t {
        low = 0,
        normal,
        high,
        critical
    };

    /**
     * @brief Concept définissant ce qu'est un message valide pour le bus.
     * Un message doit être trivialement copiable pour garantir le déterminisme
     * et doit porter une priorité.
     */
    template<typename T>
    concept event_message = std::is_trivially_copyable_v<T> && 
                            std::is_standard_layout_v<T> &&
                            requires(const T m) {
                                { m.priority } -> std::convertible_to<message_priority>;
                            };

    /**
     * @brief Interface de base pour un abonné.
     */
    template<event_message T>
    class subscriber_interface {
    public:
        virtual ~subscriber_interface() = default;
        virtual void on_message(const T& message) noexcept = 0;
    };

    /**
     * @brief Noeud d'abonné pour une liste chaînée intrusive (évite l'allocation dynamique).
     */
    template<event_message T>
    struct subscriber_node : public subscriber_interface<T> {
        subscriber_node<T>* next = nullptr;
    };

    /**
     * @brief Représente un canal de communication typé (Topic).
     * Gère la liste des abonnés et la redistribution des messages.
     */
    template<event_message T>
    class topic {
    public:
        constexpr topic() noexcept = default;

        /**
         * @brief Enregistre un abonné. 
         * Note: Pour le déterminisme, l'enregistrement doit idéalement se faire au démarrage.
         */
        void subscribe(subscriber_node<T>& node) noexcept {
            // Utilisation d'un compare-and-swap simple pour permettre l'enregistrement multi-tâches
            subscriber_node<T>* expected = m_head.load(std::memory_order_relaxed);
            do {
                node.next = expected;
            } while (!m_head.compare_exchange_weak(expected, &node, 
                                                  std::memory_order_release, 
                                                  std::memory_order_relaxed));
        }

        /**
         * @brief Diffuse un message à tous les abonnés.
         */
        void publish(const T& message) noexcept {
            subscriber_node<T>* current = m_head.load(std::memory_order_acquire);
            while (current != nullptr) {
                current->on_message(message);
                current = current->next;
            }
        }

    private:
        std::atomic<subscriber_node<T>*> m_head{nullptr};
    };

    /**
     * @brief Interface de base pour un éditeur.
     */
    template<event_message T>
    class publisher_interface {
    public:
        virtual ~publisher_interface() = default;
        virtual void publish(const T& message) noexcept = 0;
    };

    /**
     * @brief Éditeur concret lié à un Topic spécifique avec support du rate-limiting.
     * @tparam T Type de message.
     * @tparam MinIntervalUs Intervalle minimum entre deux publications (0 = illimité).
     */
    template<event_message T, uint64_t MinIntervalUs = 0>
    class publisher : public publisher_interface<T> {
    public:
        explicit publisher(topic<T>& t) noexcept : m_topic(t) {}
        
        void publish(const T& message) noexcept override {
            if constexpr (MinIntervalUs > 0) {
                uint64_t now = rtos::kernel::get_tick_count();
                uint64_t last = m_last_publish_time.load(std::memory_order_relaxed);

                // Using unsigned subtraction correctly handles 64-bit wrap-around
                if (now - last < MinIntervalUs) {
                    m_rate_limit_dropped_count.fetch_add(1, std::memory_order_relaxed);
                    return;
                }
                m_last_publish_time.store(now, std::memory_order_relaxed);
            }
            m_topic.publish(message);
        }
        size_t rate_limit_drops() const noexcept {
            return m_rate_limit_dropped_count.load(std::memory_order_relaxed);
        }

    private:
        topic<T>& m_topic;
        std::atomic<uint64_t> m_last_publish_time{0};
        std::atomic<size_t> m_rate_limit_dropped_count{0};
    };

} // namespace communication::event_bus
