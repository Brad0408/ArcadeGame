#pragma once
#include <GameObject.h>

class Bullet : public GameObject
{

public:

	Bullet(const AG::Vector2<float>& position);

	~Bullet();

	void Update(float deltaTime, GameObject* player);
	sf::CircleShape& GetCircleShape();


	void SetAngle(const AG::Vector2<float>& targetPosition);

private:
	float m_BulletRadius = 7.0f;
	float m_lifespan = 5.0f;
	AG::Vector2<float> m_velocity;

	sf::CircleShape m_BulletShape;



};
