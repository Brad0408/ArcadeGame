#pragma once
#include<ColliderComponent.h>

class BoxCollider : public ColliderComponent
{

public:
	BoxCollider(GameObject* owner);
	bool CheckCollision(const ColliderComponent& other) const override;

	//Draw an outline around the Box
	void DrawOutlines(sf::RectangleShape& shape);


	void SetBounds(sf::FloatRect &bounds);
	sf::FloatRect GetGlobalBounds() const override;

private:
	sf::FloatRect m_Bounds;
};
