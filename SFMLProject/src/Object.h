#pragma once
#include <SFML/Graphics.hpp>

class UUID;

class Object
{
public:
	//Default Constructor , uuid is also default-initalised
	Object() : uuid() {}

	//Copy Constructor, copies uuid from the source object to the destination object
	Object(const Object&) = default;

	UUID uuid;

	//Compares two Object instances, compares the uuids by casting them to a uint_64, returns true if two Objects are equal
	inline bool operator==(const Object& rhs) { return (uint64_t)this->uuid == (uint64_t)rhs.uuid; }

	//Compares two Object instances, return true if they dont match
	inline bool operator!=(const Object& rhs) { return !(*this == rhs); }

};
