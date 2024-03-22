#include "Component.h"
#include <iostream>



void Component::Start() 
{
	std::cout << "Start was called" << std::endl;
}


void Component::Update(float deltaTime)
{
	std::cout << "Update was called" << std::endl;
}


void Component::FixedUpdate(float fixedDeltaTime) 
{
	std::cout << "Fixed update was called" << std::endl;
}


void Component::OnEnable()
{
	std::cout << "On enabled was called" << std::endl;
}


void Component::OnDisable() 
{
	std::cout << "OnDisabled was called" << std::endl;
}
