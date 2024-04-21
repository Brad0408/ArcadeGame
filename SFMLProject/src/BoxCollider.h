#pragma once
#include<ColliderComponent.h>

class BoxCollider : public ColliderComponent
{

public:
	BoxCollider(GameObject* owner);
	virtual void OnCollision(ColliderComponent* otherCollider) override;


	const sf::FloatRect& GetBounds() const;

private:
	sf::FloatRect m_Bounds;
};
