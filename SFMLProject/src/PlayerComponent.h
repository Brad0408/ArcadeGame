#pragma once
#include <Component.h>
#include <GameObject.h>


class PlayerComponent : public Component
{

public:
	PlayerComponent(GameObject* owner);
	~PlayerComponent();



	AG::Vector2<float> PlayerSize = AG::Vector2<float>::one * 50;

	sf::RectangleShape PlayerShapeRectangle;

	sf::IntRect PlayerTextureUV;

	void Move();

private:
	float m_MovementSpeed = 0.1f;
};
