#include "Bullet.h"

//Create a bullet with a posisiton, velocity and the direction its moving in based of the mouse position
Bullet::Bullet(const AG::Vector2<float>& position, const float &velocity, const AG::Vector2<float> &direction)
{
	m_BulletShape.setRadius(m_BulletRadius);
	m_BulletShape.setFillColor(sf::Color::Green);

	m_position = position;
	m_velocity = velocity;
	m_direction = direction;


	m_BulletShape.setOrigin(m_BulletShape.getRadius(), m_BulletShape.getRadius());
	m_BulletShape.setPosition(m_position.x, m_position.y);
	SetName("Bullet");
	SetTag("Bullet");
	AddComponent<CircleCollider>();


	SetCircleShape(GetCircleShape());


	m_direction.Normalise();
}


void Bullet::Update(float deltaTime)
{
	//If any stray bullets get pass the walls despawn them
	m_lifespan -= deltaTime;

	//Update its position
	m_position += m_direction * m_velocity * deltaTime;
	m_BulletShape.setPosition(m_position);


	if (m_lifespan <= 0.0)
	{
		std::cout << "Bullet Expired" << std::endl;
		if (this)
		{
			this->MarkForRemoval();
		}
	}

}

sf::CircleShape& Bullet::GetCircleShape()
{
	return m_BulletShape;
}

