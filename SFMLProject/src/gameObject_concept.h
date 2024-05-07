#pragma once
#include <concepts>
#include <GameObject.h>

//Template to make sure only a componenet is being used
template<class T>
concept isGameObject = std::is_base_of<GameObject, T>::value;
