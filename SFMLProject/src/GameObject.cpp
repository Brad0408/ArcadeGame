#include "GameObject.h"



GameObject::GameObject()
{
	//std::cout << "GameObject Constructed" << std::endl;
}

GameObject::~GameObject()
{
	
}



void GameObject::Update(float deltaTime)
{
}

void GameObject::Render(sf::RenderWindow& window)
{
}

void GameObject::SetActive(bool newActive)
{
}



void GameObject::SetTexture(sf::Texture* texture)
{
	m_SpriteShape.setTexture(texture);

	std::cout << "Texture Address: " << m_SpriteShape.getTexture() << std::endl;
}

const sf::Texture* GameObject::GetTexture()
{
	return m_SpriteShape.getTexture();
}

void GameObject::SetTextureRect(sf::IntRect &textureRect)
{
	m_SpriteShape.setTextureRect(textureRect);
}

sf::IntRect GameObject::GetTextureRect()
{
	return m_SpriteShape.getTextureRect();
}

void GameObject::SetRectangleShape(sf::RectangleShape &shape)
{
	m_SpriteShape = shape;
}

sf::RectangleShape& GameObject::GetRectangleShape()
{
	return m_SpriteShape;
}

void GameObject::SetLocation(float x, float y)
{
	m_location.x = x;
	m_location.y = y;

	//std::cout << "Location: " << x << ":" << y << std::endl;
}

AG::Vector2<float>& GameObject::GetLocation()
{
	return m_location;
}

void GameObject::DrawOutlines(sf::RectangleShape &shape)
{
	shape.setOutlineThickness(3);
	shape.setOutlineColor(sf::Color(255, 0, 255));
}

void GameObject::CheckCollisions(GameObject* other)
{
	//std::cout << "Check was called " << std::endl;

	ColliderComponent* collider1 = GetComponent<ColliderComponent>();
	ColliderComponent* collider2 = other->GetComponent<ColliderComponent>();

	// Check if both GameObjects have colliders
	if (collider1 && collider2) {
		// Check if the colliders intersect
		if (dynamic_cast<BoxCollider*>(collider1) && dynamic_cast<BoxCollider*>(collider2)) {
			BoxCollider* boxCollider1 = static_cast<BoxCollider*>(collider1);
			BoxCollider* boxCollider2 = static_cast<BoxCollider*>(collider2);

			if (boxCollider1->GetBounds().intersects(boxCollider2->GetBounds())) {
				// Handle collision
				collider1->OnCollision(collider2);
				collider2->OnCollision(collider1);
			}
		}
	}
}
