#pragma once
#include <GameObject.h>

class Bullet : public GameObject
{

public:

	Bullet(const AG::Vector2<float>& position, const float& velocity, const AG::Vector2<float> &direction);
	~Bullet();

	void Update(float deltaTime);
	sf::CircleShape& GetCircleShape();

	void MarkForRemoval();
	bool ShouldRemove();

private:
	float m_BulletRadius = 7.0f;
	float m_lifespan = 10.0f;
	float m_velocity;

	AG::Vector2<float> m_position;
	AG::Vector2<float>  m_direction;

	sf::CircleShape m_BulletShape;

	bool m_shouldRemove;

};
