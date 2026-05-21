#pragma once
#include <concepts>
#include <type_traits>

template <typename T>
concept TxMessageType = std::is_trivial_v<T>;