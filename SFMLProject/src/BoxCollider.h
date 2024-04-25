#pragma once
#include <ColliderComponent.h>
#include <GameObject.h>

class BoxCollider : public ColliderComponent
{

public:
	BoxCollider(GameObject* owner);

	~BoxCollider();

	bool CheckCollision(GameObject* objectOne, GameObject* objectTwo)  override;

	static bool WallCollision(GameObject* objectOne, GameObject* objectTwo);


	//Draw an outline around the Box
	void DrawOutlines(sf::RectangleShape& shape);


	void SetGlobalBounds(sf::FloatRect &bounds);
	sf::FloatRect GetGlobalBounds() override;

private:
	sf::FloatRect m_Bounds;
};
