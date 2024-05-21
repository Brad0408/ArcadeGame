#include "Component.h"

void Component::Update(float deltaTime)
{
	std::cout << "Update was called" << std::endl;
}

void Component::FixedUpdate(float fixedDeltaTime) 
{
	std::cout << "Fixed update was called" << std::endl;
}
