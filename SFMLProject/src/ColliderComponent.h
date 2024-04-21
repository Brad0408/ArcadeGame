#pragma once
#include <Component.h>


class ColliderComponent : public Component
{

public:
	ColliderComponent(GameObject* owner);
	virtual void OnCollision(ColliderComponent* otherCollider) = 0;

};
