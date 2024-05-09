#pragma once
#include <Component.h>
#include <GameObject.h>


class EnemyComponent : public Component
{

public:
	EnemyComponent(GameObject* owner);
	~EnemyComponent();


	void Move(float deltaTime);
	void Update(float deltaTime) override;

private:
	AG::Vector2<float> m_EnemySize = AG::Vector2<float>::one * 10;

	sf::RectangleShape m_EnemyShapeRectangle;

	sf::IntRect m_EnemyTextureUV;

	float m_MovementSpeed = 37.0f;
};
