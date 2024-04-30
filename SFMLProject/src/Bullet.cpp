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

