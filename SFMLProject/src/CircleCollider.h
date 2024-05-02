#pragma once
#include <ColliderComponent.h>
#include <GameObject.h>

class CircleCollider : public ColliderComponent
{

public:
	CircleCollider(GameObject* owner);

	~CircleCollider();

	bool BulletCollision(Bullet* circleObject, GameObject* objectTwo);

	static bool WallCollision(GameObject* objectOne, GameObject* objectTwo);


	//Draw an outline around the Box
	void DrawOutlines(sf::CircleShape& shape);


	void SetGlobalBounds(sf::FloatRect& bounds);
	sf::FloatRect GetGlobalBounds() override;

private:
	sf::FloatRect m_Bounds;
};
