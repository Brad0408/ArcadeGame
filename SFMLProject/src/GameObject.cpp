#include "GameObject.h"



GameObject::GameObject()
{
	//std::cout << "GameObject Constructed" << std::endl;
}

GameObject::~GameObject()
{
	
}


void GameObject::SetRectangleShape(sf::RectangleShape &shape)
{
	m_SpriteShape = shape;
}

sf::RectangleShape& GameObject::GetRectangleShape()
{
	return m_SpriteShape;
}

void GameObject::SetCircleShape(sf::CircleShape& shape)
{
	m_CircleShape = shape;
}

sf::CircleShape& GameObject::GetCircleShape()
{
	return m_CircleShape;
}

void GameObject::SetLocation(float x, float y)
{
	m_location.x = x;
	m_location.y = y;

	m_SpriteShape.setPosition(m_location.x, m_location.y);
	//std::cout << "Location: " << x << ":" << y << std::endl;
}

AG::Vector2<float>& GameObject::GetLocation()
{
	return m_location;
}


AG::Vector2<float> &GameObject::GetMoveDirection()
{
	return m_MoveDirection;
}

void GameObject::SetIsWall(bool isWall)
{
	m_IsWall = isWall;
}

bool GameObject::GetIsWall()
{
	return m_IsWall;
}

void GameObject::SetIsPlayer(bool isPlayer)
{
	m_IsPlayer = isPlayer;
}

bool GameObject::GetIsPlayer()
{
	return m_IsPlayer;
}

void GameObject::SetIsEnemy(bool isEnemy)
{
	m_IsEnemy = isEnemy;
}

bool GameObject::GetIsEnemy()
{
	return m_IsEnemy;
}

void GameObject::SetIsShooting(bool isShooting)
{
	m_IsShooting = isShooting;
}

bool GameObject::GetIsShooting()
{
	return m_IsShooting;
}

void GameObject::MarkForRemoval()
{
	m_shouldRemove = true;
}

bool GameObject::ShouldRemove()
{
	return m_shouldRemove;
}
