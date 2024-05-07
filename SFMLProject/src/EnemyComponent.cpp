#include "EnemyComponent.h"

EnemyComponent::EnemyComponent(GameObject* owner) : Component(owner)
{
	_GameObject->SetLocation(450, 450);


	//Cookie cutter part of sprite sheet (0,0 = Coordinates, 24, 24 = Size of rectangle)
	m_EnemyTextureUV = sf::IntRect(148, 235, 24, 24);

	m_EnemyShapeRectangle.setSize(m_EnemySize);



	//Set texture to be the whole sprite sheet
	m_EnemyShapeRectangle.setTexture(&ResourceManager::GetTexture("Player"));


	//Set the texture to the cookie cutter section of the sprite sheet
	m_EnemyShapeRectangle.setTextureRect(m_EnemyTextureUV);


	//Set Rect to the middle / position
	m_EnemyShapeRectangle.setOrigin(m_EnemySize / 2);
	m_EnemyShapeRectangle.setPosition(_GameObject->GetLocation());


	//_GameObject->DrawOutlines(PlayerShapeRectangle);


	//Actually set it
	_GameObject->SetRectangleShape(m_EnemyShapeRectangle);










}

EnemyComponent::~EnemyComponent()
{
}
void EnemyComponent::Update(float deltaTime)
{
	Move(deltaTime);
}

void EnemyComponent::Move(float deltaTime)
{
	AG::Vector2<float> direction(0.0f, 0.0f);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		_GameObject->GetLocation().x -= m_MovementSpeed * deltaTime;

		direction.x = -1.0f;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		_GameObject->GetLocation().x += m_MovementSpeed * deltaTime;

		direction.x = 1.0f;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		_GameObject->GetLocation().y -= m_MovementSpeed * deltaTime;

		direction.y = -1.0f;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		_GameObject->GetLocation().y += m_MovementSpeed * deltaTime;

		direction.y = 1.0f;
	}

	direction.Normalise();

	_GameObject->GetMoveDirection() = direction;

	_GameObject->GetRectangleShape().setPosition(_GameObject->GetLocation());
}
