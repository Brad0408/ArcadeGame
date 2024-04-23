#include "BoxCollider.h"

BoxCollider::BoxCollider(GameObject* owner) : ColliderComponent (owner)
{
}

BoxCollider::~BoxCollider()
{
}





bool BoxCollider::CheckCollision(const ColliderComponent& other) const
{
	//std::cout << "Box Collider On Collision Called " << std::endl;

	// Downcast the other collider to BoxCollider
	const BoxCollider* otherBoxCollider = dynamic_cast<const BoxCollider*>(&other);
	if (otherBoxCollider) 
	{
		// Use SFML's intersects method to check collision
		return m_Bounds.intersects(otherBoxCollider->GetGlobalBounds());

		std::cout << "There has been an intersection" << std::endl;
	}
	return false; // Return false if the other collider is not a BoxCollider
}


void BoxCollider::DrawOutlines(sf::RectangleShape& shape)
{
	shape.setOutlineThickness(3);
	shape.setOutlineColor(sf::Color(0, 0, 255));
}





void BoxCollider::SetBounds(sf::FloatRect &bounds) 
{
	m_Bounds = bounds;
}

sf::FloatRect BoxCollider::GetGlobalBounds() const
{
	return m_Bounds;
}

