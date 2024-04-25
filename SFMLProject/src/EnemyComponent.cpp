#include "EnemyComponent.h"

EnemyComponent::EnemyComponent(GameObject* owner) : Component(owner)
{
	_GameObject->SetLocation(450, 450);
}

EnemyComponent::~EnemyComponent()
{
}

void EnemyComponent::Move()
{
	AG::Vector2<float> direction(0.0f, 0.0f);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		_GameObject->GetLocation().x -= m_MovementSpeed;

		direction.x = -1.0f;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		_GameObject->GetLocation().x += m_MovementSpeed;

		direction.x = 1.0f;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		_GameObject->GetLocation().y -= m_MovementSpeed;

		direction.y = -1.0f;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		_GameObject->GetLocation().y += m_MovementSpeed;

		direction.y = 1.0f;
	}

	direction.Normalise();

	_GameObject->GetMoveDirection() = direction;

	_GameObject->GetRectangleShape().setPosition(_GameObject->GetLocation());
}
