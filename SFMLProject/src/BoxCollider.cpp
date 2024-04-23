#include "BoxCollider.h"

BoxCollider::BoxCollider(GameObject* owner) : ColliderComponent (owner)
{
}

BoxCollider::~BoxCollider()
{
}




bool BoxCollider::CheckCollision(GameObject* objectOne, GameObject* objectTwo)
{




	return false;
}








void BoxCollider::DrawOutlines(sf::RectangleShape& shape)
{
	shape.setOutlineThickness(3);
	shape.setOutlineColor(sf::Color(0, 0, 255));
}





void BoxCollider::SetGlobalBounds(sf::FloatRect &bounds) 
{
	m_Bounds = bounds;
}

sf::FloatRect BoxCollider::GetGlobalBounds()
{
	return m_Bounds;
}

