#include "Bullet.h"


Bullet::Bullet(const AG::Vector2<float>& position, const float &velocity, const AG::Vector2<float> &direction)
{
	m_BulletShape.setRadius(m_BulletRadius);
	m_BulletShape.setFillColor(sf::Color::Green);

	m_position = position;
	m_velocity = velocity;
	m_direction = direction;

	m_BulletShape.setOrigin(m_BulletShape.getRadius(), m_BulletShape.getRadius());
	m_BulletShape.setPosition(m_position.x , m_position.y );
	SetName("Bullet");
	AddComponent<CircleCollider>();


	SetCircleShape(GetCircleShape());


	m_direction.Normalise();

	//m_shouldRemove = false;
}

Bullet::~Bullet()
{

}

void Bullet::Update(float deltaTime)
{
	m_lifespan -= deltaTime;


	m_position += m_direction * m_velocity * deltaTime;


	m_BulletShape.setPosition(m_position);


	if (m_lifespan <= 0.0)
	{
		std::cout << "Expired" << std::endl;
		//GameManager::RemoveBullet(this);
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

//void Bullet::MarkForRemoval()
//{
//	m_shouldRemove = true;
//}
//
//bool Bullet::ShouldRemove()
//{
//	return m_shouldRemove;
//}

