#include "ColliderComponent.h"

ColliderComponent::ColliderComponent(GameObject* owner) : Component(owner)
{
}

bool ColliderComponent::CheckCollision(GameObject* objectOne, GameObject* objectTwo)
{
    return false;
}
