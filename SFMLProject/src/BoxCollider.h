#pragma once
#include <ColliderComponent.h>
#include <GameObject.h>

//Collider used by anything other than a bullet (player, enemies, family, walls)
class BoxCollider : public ColliderComponent
{

public:
	BoxCollider(GameObject* owner);

	~BoxCollider() {}

	bool CheckCollision(GameObject* objectOne, GameObject* objectTwo) override;

	static bool WallCollision(GameObject* objectOne, GameObject* objectTwo);

	void DrawOutlines(sf::RectangleShape& shape);

};
