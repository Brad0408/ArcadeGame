#include "Bullet.h"


Bullet::Bullet()
{
	m_BulletShape.setRadius(m_BulletRadius);
	m_BulletShape.setFillColor(sf::Color::Green);

	std::cout << "Bullet Instantiated" << std::endl;
}

Bullet::~Bullet()
{
}

sf::CircleShape& Bullet::GetCircleShape()
{
	//std::cout << "Claled uinfudnasdas " << std::endl;

	return m_BulletShape;
}

