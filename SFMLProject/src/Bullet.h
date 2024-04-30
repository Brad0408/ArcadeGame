#pragma once
#include <GameObject.h>

class Bullet : public GameObject
{

public:

	Bullet();

	~Bullet();




private:
	float m_BulletRadius = 10.0f;
	AG::Vector2<float> m_velocity;

	sf::CircleShape m_BulletShape;



};
