#include "CircleCollider.h"
#include "Bullet.h"

CircleCollider::CircleCollider(GameObject* owner) : ColliderComponent(owner)
{
}

//Collision check done with shape intersects, but with a bullet object and a regular gameobject
bool CircleCollider::BulletCollision(Bullet* circleObject, GameObject* rectObject)
{
	sf::CircleShape circle = circleObject->GetCircleShape();
	sf::RectangleShape rect = rectObject->GetRectangleShape();

	if (circle.getGlobalBounds().intersects(rect.getGlobalBounds()))
	{
		//True if the intersect
		return true;
	}

	return false;
}


void CircleCollider::DrawOutlines(sf::CircleShape& shape)
{
	shape.setOutlineThickness(2);
	shape.setOutlineColor(sf::Color(0, 0, 255));
}
