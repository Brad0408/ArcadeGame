#pragma once
#include <GameObject.h>

class Enemy : public GameObject
{

public:
	Enemy(const AG::Vector2<float>& spawnLocation);
	~Enemy();

	void Update(float deltaTime, const AG::Vector2<float>& playerPosition) override;
	void Move(AG::Vector2<float>& displacement);

private:
	AG::Vector2<float> m_EnemySize = AG::Vector2<float>::one * 35;

	sf::RectangleShape m_EnemyShapeRectangle;

	sf::IntRect m_EnemyTextureUV;

	float m_MovementSpeed = 10.0f;

};
