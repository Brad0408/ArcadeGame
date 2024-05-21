#include "GameObject.h"



GameObject::GameObject()
{
	//std::cout << "GameObject Constructed" << std::endl;
}


void GameObject::SetRectangleShape(sf::RectangleShape &shape)
{
	m_SpriteShape = shape;
}


void GameObject::SetCircleShape(sf::CircleShape& shape)
{
	m_CircleShape = shape;
}

void GameObject::SetLocation(float x, float y)
{
	m_location.x = x;
	m_location.y = y;

	m_SpriteShape.setPosition(m_location.x, m_location.y);
}

void GameObject::SetIsShooting(bool isShooting)
{
	m_IsShooting = isShooting;
}


//Used for cleanup - In GameManager.h is RemoveMarkedObjectsList() which is checking for this bool switch
void GameObject::MarkForRemoval()
{
	m_shouldRemove = true;
}

