#ifndef RTOS_ABSTRACT_HPP
#define RTOS_ABSTRACT_HPP

namespace RtosAbstract {

	/**
	 * @brief Classe abstraite ou interface commune pour un Mutex.
	 * Tous les wrappers RTOS doivent implémenter ces méthodes.
	 */
	class IMutex {
	public:
		virtual ~IMutex() = default;
		virtual void lock() = 0;
		virtual void unlock() = 0;
		// virtual bool try_lock(uint32_t timeout_ms) = 0; // Pour une abstraction plus complète
	};

	/**
	 * @brief Le LockGuard utilisant le principe RAII.
	 * Il ne dépend que de l'interface IMutex.
	 */
	class LockGuard {
	public:
		explicit LockGuard(IMutex& mutex)
			: m_mutex(mutex) {
			m_mutex.lock();
		}
		~LockGuard() {
			m_mutex.unlock();
		}
    
		// Empêche la copie
		LockGuard(const LockGuard&) = delete;
		LockGuard& operator=(const LockGuard&) = delete;

	private:
		IMutex& m_mutex;
	};

} // namespace RtosAbstract

#endif // RTOS_ABSTRACT_HPP