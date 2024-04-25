#pragma once
#include <Component.h>


class ColliderComponent : public Component
{

public:
	ColliderComponent(GameObject* owner);
	
	~ColliderComponent();


	virtual bool CheckCollision(GameObject* objectOne, GameObject* objectTwo);

	virtual sf::FloatRect GetGlobalBounds();

};
