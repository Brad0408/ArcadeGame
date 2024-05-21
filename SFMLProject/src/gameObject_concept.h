#pragma once
#include <concepts>
#include <GameObject.h>

//For removal of gameObjects
template<class T>
concept isGameObject = std::is_base_of<GameObject, T>::value;
