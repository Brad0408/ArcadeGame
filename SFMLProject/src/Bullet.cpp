#include "Bullet.h"


Bullet::Bullet(const AG::Vector2<float>& position)
{
	m_BulletShape.setRadius(m_BulletRadius);
	m_BulletShape.setFillColor(sf::Color::Green);
	m_BulletShape.setPosition(position.x - m_BulletShape.getRadius(), position.y - m_BulletShape.getRadius());
	SetName("Bullet");
	AddComponent<CircleCollider>();


	SetCircleShape(GetCircleShape());
}

Bullet::~Bullet()
{
	GameManager::RemoveBullet(this);
}

void Bullet::Update(float deltaTime, GameObject* player)
{
	m_lifespan -= deltaTime;

	if (m_lifespan <= 0.0)
	{
		GameManager::RemoveBullet(this);
	}

}

sf::CircleShape& Bullet::GetCircleShape()
{
	return m_BulletShape;
}

void Bullet::SetAngle(const AG::Vector2<float>& targetPosition)
{

}

