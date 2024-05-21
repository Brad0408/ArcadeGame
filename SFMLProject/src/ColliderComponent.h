#pragma once
#include <Component.h>

//Base collider component for the box and circle colliders
class ColliderComponent : public Component
{

public:
	ColliderComponent(GameObject* owner);
	
	~ColliderComponent() {}

	virtual bool CheckCollision(GameObject* objectOne, GameObject* objectTwo);
};
