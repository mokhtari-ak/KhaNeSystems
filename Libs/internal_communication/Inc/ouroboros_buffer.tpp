#pragma once

namespace communication::event_bus {

    template<event_message T, size_t Capacity>
    void ouroboros_buffer<T, Capacity>::push(const T& message) noexcept {
        // Use mutex to protect the entire push operation (indices + data)
        // Note: Using a non-blocking lock to avoid priority inversion and deadlock in high-freq tasks
        if (!m_mutex.try_lock(0)) {
            // Contention: for this specific architecture, we drop if busy to remain deterministic
            m_overwrite_dropped_count.fetch_add(1, std::memory_order_relaxed);
            return;
        }

        size_t current_size = m_size.load(std::memory_order_relaxed);
        
        if (current_size >= Capacity) {
            size_t head_idx = m_head.load(std::memory_order_relaxed);
            
            if (static_cast<uint8_t>(message.priority) < static_cast<uint8_t>(m_data[head_idx].priority)) {
                m_overwrite_dropped_count.fetch_add(1, std::memory_order_relaxed);
                m_mutex.unlock();
                return;
            }
            
            m_head.store((head_idx + 1) % Capacity, std::memory_order_relaxed);
            m_overwrite_dropped_count.fetch_add(1, std::memory_order_relaxed);
        } else {
            m_size.fetch_add(1, std::memory_order_relaxed);
        }

        size_t current_tail = m_tail.load(std::memory_order_relaxed);
        m_data[current_tail] = message;
        m_tail.store((current_tail + 1) % Capacity, std::memory_order_relaxed);
        
        m_mutex.unlock();
    }

    template<event_message T, size_t Capacity>
    bool ouroboros_buffer<T, Capacity>::pop(T& out_message) noexcept {
        m_mutex.lock(); // pop is usually from a consumer task, blocking is acceptable
        
        size_t current_size = m_size.load(std::memory_order_relaxed);
        if (current_size == 0) {
            m_mutex.unlock();
            return false;
        }

        size_t current_head = m_head.load(std::memory_order_relaxed);
        out_message = m_data[current_head];
        
        m_head.store((current_head + 1) % Capacity, std::memory_order_relaxed);
        m_size.fetch_sub(1, std::memory_order_relaxed);
        
        m_mutex.unlock();
        return true;
    }

} // namespace communication::event_bus
