#pragma once
#include <Component.h>


class ColliderComponent : public Component
{

public:
	ColliderComponent(GameObject* owner);
	
	~ColliderComponent();


	virtual bool CheckCollision(const ColliderComponent& other) const = 0;

	virtual sf::FloatRect GetGlobalBounds() const = 0;

};
