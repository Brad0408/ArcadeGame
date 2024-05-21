#pragma once
#include <ColliderComponent.h>
#include <GameObject.h>

//Collider used by the bullets
class CircleCollider : public ColliderComponent
{

public:
	CircleCollider(GameObject* owner);

	~CircleCollider() {}

	bool BulletCollision(Bullet* circleObject, GameObject* objectTwo);

	void DrawOutlines(sf::CircleShape& shape);
};
