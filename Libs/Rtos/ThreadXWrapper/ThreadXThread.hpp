#pragma once
#include "Thread.hpp" // L'interface IThread
#include <cstdint>
#include <functional>                    
#include "tx_api.h"
#include <map>
#include "ThreadTypes.hpp"
#include "ThreadXConfig.hpp"
#include <type_traits>
#include <stdexcept>

// namespace Rtos::ThreadXWrapper
namespace Rtos::ThreadXWrapper {
	extern TX_BYTE_POOL application_byte_pool;
	/**
	 * @brief Implémentation concrète de l'interface IThread utilisant ThreadX.
	 */
	template <Rtos::Abstract::ThreadStaticConfigConcept Config>
	class ThreadXThread final : public Rtos::Abstract::IThread {
	public:
		/**
		 * @brief Construit un objet ThreadXThread.
		 */
		ThreadXThread() {
			create();
		}

		/**
		 * @brief Destructeur. Nettoie les ressources associées au thread.
		 */
		~ThreadXThread() override{
			// Nettoyage des ressources
			destroy();
		}
		
		// Suppression de la copie et de l'affectation pour garantir l'unicité du thread statique.
		ThreadXThread(const ThreadXThread&) = delete;
		ThreadXThread& operator=(const ThreadXThread&) = delete;

	  protected:

		/**
		 * @brief Démarre l'exécution du thread ThreadX.
		 */
		void create() override 
		{
			// Implémentation spécifique à ThreadX pour démarrer un thread
			UINT status;

			// 1. ALLOCATION de la pile (tx_byte_allocate)
			status = tx_byte_allocate(
				&application_byte_pool,
				&m_stack_start_ptr, // Adresse de la pile allouée
				Config::StackSize,
				TX_WAIT_FOREVER);

			if (status != TX_SUCCESS)
			{
				throw std::runtime_error("Erreur: Allocation de pile echouee pour Thread " + std::string(Config::Name));
			}

			// 2. CRÉATION du thread (tx_thread_create)
			status = tx_thread_create(
				&m_thread,						  // Bloc de contrôle du thread
				const_cast<CHAR *>(Config::Name), // Nom
				c_entry_wrapper,				  // Fonction wrapper statique C
				(ULONG)Config::ThreadId,		  // Input (passer l'ID statique)
				m_stack_start_ptr,				  // Adresse de la pile
				Config::StackSize,				  // Taille de la pile
				Config::Priority,				  // Priorité
				Config::PriorityThreshold,		  // Seuil de préemption
				TX_NO_TIME_SLICE,
				Config::AutoStart ? TX_AUTO_START : TX_DONT_START // Démarrage
			);

			if (status != TX_SUCCESS)
			{
				// Nettoyer si la création échoue
				tx_byte_release(m_stack_start_ptr);
				throw std::runtime_error("Erreur: Creation de thread ThreadX echouee.");
			}
		}

		void destroy() override
		{
			// Supprimer le thread du système ThreadX
			(void)tx_thread_delete(&m_thread);

			// Libérer la mémoire de la pile allouée
			if (m_stack_start_ptr != nullptr)
			{
				tx_byte_release(m_stack_start_ptr);
			}
		}

		void start() noexcept override {
			(void)tx_thread_resume(&m_thread);
		}

		void stop() noexcept override {
			(void)tx_thread_suspend(&m_thread);
		}

	private:
			TX_THREAD m_thread{};
			// Pointeur vers la mémoire de la pile allouée dynamiquement au runtime
			VOID *m_stack_start_ptr = nullptr;

			/**
			 * @brief Wrapper C statique qui fait le lien avec la fonction d'entrée C++.
			 * ThreadX exige une fonction avec la signature 'void func(ULONG)'.
			 * Nous utilisons l'input ULONG pour passer un identifiant si nécessaire.
			 */
			static void c_entry_wrapper(ULONG thread_input)
			{
				(void)thread_input; // Input non utilisé car la fonction est statique

				// Appelle la fonction C++ stockée dans le template struct
				Config::EntryFunction();

				// Note: Si le thread sort de cette fonction, ThreadX le mettra en état Terminated.
			}
		};
}
