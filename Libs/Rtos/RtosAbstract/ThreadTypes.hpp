#pragma once
#include <cstdint>
#include <functional>	
#include <type_traits>
#include <vector>
#include <concepts>
#include <map>

namespace Rtos::Abstract
{
	template <typename T>
	concept StdFunction = std::is_invocable_r_v<void, T>; // void type de retour, (*) pointeur de fonction, () pas de paramètre

	template <StdFunction auto entryFunction = [] {}, uint8_t threadId = 0, const char *name = nullptr,
			  int priority = 0, int priority_threshold = 0, uint32_t stackSize = 1024, bool autostart = false>
	struct ThreadStaticConfig {
		uint8_t constexpr static ThreadId = threadId;
		static constexpr auto EntryFunction = entryFunction;
		static constexpr const char* Name = name;
		static constexpr int Priority = priority;
		static constexpr int PriorityThreshold = priority_threshold;
		static constexpr uint32_t StackSize = stackSize;
		static constexpr bool AutoStart = autostart;
	};

	template <typename T>
	concept ThreadStaticConfigConcept = requires {
		{ T::ThreadId } -> std::convertible_to<uint8_t>;
		{ T::EntryFunction };
		{ T::Priority } -> std::convertible_to<int>;
		{ T::PriorityThreshold } -> std::convertible_to<int>;
		{ T::StackSize } -> std::convertible_to<uint32_t>;
		{ T::AutoStart } -> std::convertible_to<bool>;
	};
}
