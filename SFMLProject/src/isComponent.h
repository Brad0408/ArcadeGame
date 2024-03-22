#pragma once
#include <concepts>
#include <Component.h>

//Template to make sure only a componenet is being used
template<class T>
concept isComponent = std::derived_from<T, Component> == true;
