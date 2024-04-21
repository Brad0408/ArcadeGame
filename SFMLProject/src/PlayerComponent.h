#pragma once
#include <Component.h>
#include <GameObject.h>


class PlayerComponent : public Component
{

public:
	PlayerComponent(GameObject* owner);

	AG::Vector2<float> PlayerSize = AG::Vector2<float>::one * 100;

	sf::RectangleShape PlayerShapeRectangle;

	void Move();

private:
	float m_MovementSpeed = 0.05f;
};
