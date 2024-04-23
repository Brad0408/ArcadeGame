#pragma once
#include<ColliderComponent.h>

class BoxCollider : public ColliderComponent
{

public:
	BoxCollider(GameObject* owner);

	~BoxCollider();

	bool CheckCollision(GameObject* objectOne, GameObject* objectTwo)  override;




	//Draw an outline around the Box
	void DrawOutlines(sf::RectangleShape& shape);


	void SetGlobalBounds(sf::FloatRect &bounds);
	sf::FloatRect GetGlobalBounds() override;

private:
	sf::FloatRect m_Bounds;
};
