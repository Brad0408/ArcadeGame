#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <UUID.h>
#include <iostream>
#include <Vector2.h>
#include <unordered_map>
#include <array>
#include <string>
#include <memory>
#include <cmath>
#include <random>
#include <chrono>


class Object
{
public:
	//Default Constructor , uuid is also default-initalised
	Object() : uuid() {}

	//Copy Constructor, copies uuid from the source object to the destination object
	Object(const Object&) = default;

	AG::UUID uuid;

	inline bool operator==(const Object& rhs) { return (uint64_t)this->uuid == (uint64_t)rhs.uuid; }
	inline bool operator!=(const Object& rhs) { return !(*this == rhs); }

};
