#pragma once
#include <concepts>

template<typename T>

//if 'requires' is used arithmetic must have the matching data types
concept arithmetic = std::integral<T> or std::floating_point<T>;
