#include "ColliderComponent.h"

ColliderComponent::ColliderComponent(GameObject* owner) : Component(owner)
{
}

ColliderComponent::~ColliderComponent()
{
}

bool ColliderComponent::CheckCollision(GameObject* objectOne, GameObject* objectTwo)
{
    return false;
}

sf::FloatRect ColliderComponent::GetGlobalBounds()
{
	return sf::FloatRect();
}
