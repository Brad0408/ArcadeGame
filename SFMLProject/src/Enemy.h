#pragma once
#include <GameObject.h>

class Enemy : public GameObject
{

public:
	Enemy(const AG::Vector2<float>& spawnLocation);
	~Enemy();


private:
	AG::Vector2<float> m_EnemySize = AG::Vector2<float>::one * 35;

	sf::RectangleShape m_EnemyShapeRectangle;

	sf::IntRect m_EnemyTextureUV;

	float m_MovementSpeed = 37.0f;

};
