#include "CircleCollider.h"
#include "Bullet.h"

CircleCollider::CircleCollider(GameObject* owner) : ColliderComponent(owner)
{
}

CircleCollider::~CircleCollider()
{
}

bool CircleCollider::BulletCollision(Bullet* circleObject, GameObject* rectObject)
{
	sf::CircleShape circle = circleObject->GetCircleShape();
	sf::RectangleShape rect = rectObject->GetRectangleShape();

	// Check if the circle intersects with the rectangle
	if (circle.getGlobalBounds().intersects(rect.getGlobalBounds()))
	{
		// If there is an intersection, return true
		return true;
	}

	// If there is no intersection, return false
	return false;
}

void CircleCollider::DrawOutlines(sf::CircleShape& shape)
{
	shape.setOutlineThickness(3);
	shape.setOutlineColor(sf::Color(0, 0, 255));
}

void CircleCollider::SetGlobalBounds(sf::FloatRect& bounds)
{
}

sf::FloatRect CircleCollider::GetGlobalBounds()
{
	return m_Bounds;
}
