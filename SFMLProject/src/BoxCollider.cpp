#include "BoxCollider.h"

BoxCollider::BoxCollider(GameObject* owner) : ColliderComponent (owner)
{




}

void BoxCollider::OnCollision(ColliderComponent* otherCollider)
{
	std::cout << "Box Collider On Collision Called " << std::endl;
}




const sf::FloatRect& BoxCollider::GetBounds() const
{
	return m_Bounds;
}
