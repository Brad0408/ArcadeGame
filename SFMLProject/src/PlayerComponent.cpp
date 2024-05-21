#include "PlayerComponent.h"
#include "Bullet.h"


PlayerComponent::PlayerComponent(GameObject* owner) : Component(owner)
{
	_GameObject->AddComponent<BoxCollider>();
	_GameObject->AddComponent<AnimationComponent>();
	_GameObject->SetName("Player");
	_GameObject->SetTag("Player");

	_GameObject->SetLocation(500, 450);


	animationComponent = _GameObject->GetComponent<AnimationComponent>();
	


	//Cookie cutter part of sprite sheet (0,0 = Coordinates, 24, 24 = Size of rectangle)
	m_PlayerTextureUV = sf::IntRect(342, 164, 24, 24);
	m_PlayerShapeRectangle.setSize(m_PlayerSize);
	m_PlayerShapeRectangle.setTexture(&ResourceManager::GetTexture("Player"));
	m_PlayerShapeRectangle.setTextureRect(m_PlayerTextureUV);
	m_PlayerShapeRectangle.setOrigin(m_PlayerSize / 2);
	m_PlayerShapeRectangle.setPosition(_GameObject->GetLocation());
	_GameObject->SetRectangleShape(m_PlayerShapeRectangle);



	CreateFiringPoint();

	//const sf::IntRect* textureRectPtr = &_GameObject->GetRectangleShape().getTextureRect();
	//std::cout << "Texture Rect Address: " << static_cast<const void*>(textureRectPtr) << std::endl;
	// 
	//sf::IntRect textureRect = _GameObject->GetTextureRect();
	//std::cout << "Texture Rect: left=" << textureRect.left << ", top=" << textureRect.top << ", width=" << textureRect.width << ", height=" << textureRect.height << std::endl;

}

void PlayerComponent::Update(float deltaTime)
{
	Move(deltaTime);

	m_timeSinceLastShot += deltaTime;

	//_GameObject->GetComponent<BoxCollider>()->DrawOutlines(_GameObject->GetRectangleShape());
}



void PlayerComponent::Move(float deltaTime)
{
	AG::Vector2<float> direction(0.0f, 0.0f);


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		animationComponent->SetPlayerAnimation(AnimationComponent::PlayerStates::Left);

		_GameObject->GetLocation().x -= m_MovementSpeed * deltaTime;
		direction.x = -1.0f;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		animationComponent->SetPlayerAnimation(AnimationComponent::PlayerStates::Right);

		_GameObject->GetLocation().x += m_MovementSpeed * deltaTime;
		direction.x = 1.0f;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		animationComponent->SetPlayerAnimation(AnimationComponent::PlayerStates::Up);

		_GameObject->GetLocation().y -= m_MovementSpeed * deltaTime;
		direction.y = -1.0f;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		animationComponent->SetPlayerAnimation(AnimationComponent::PlayerStates::Down);

		_GameObject->GetLocation().y += m_MovementSpeed * deltaTime;
		direction.y = 1.0f;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		//GameManager::RestartGame();
	}
	else
	{
		animationComponent->SetPlayerAnimation(AnimationComponent::PlayerStates::Idle);
	}


	m_PlayerShapeRectangle.setTextureRect(animationComponent->GetCurrentFrame(animationComponent->GetPlayerState(), deltaTime, animationComponent->GetPlayerAnimationsMap()));
	_GameObject->SetRectangleShape(m_PlayerShapeRectangle);

	direction.Normalise();

	_GameObject->GetMoveDirection() = direction;
	_GameObject->GetRectangleShape().setPosition(_GameObject->GetLocation());
}


#pragma region ShooingAndAiming

//Creates the red square that circles the player
void PlayerComponent::CreateFiringPoint()
{
	m_FiringPoint.setSize(m_FiringPointSize);
	m_FiringPoint.setFillColor(sf::Color::Red);
	m_FiringPoint.setOrigin(m_FiringPoint.getSize().x / 2.0f, m_FiringPoint.getSize().y / 2.0f);
}


void PlayerComponent::CalculateFiringPointRotation(sf::RenderWindow &window)
{
	//Calculate angle between player and mouse cursor
	m_MousePosition = AG::Vector2<float>(sf::Mouse::getPosition(window));
	AG::Vector2<float>  delta = m_MousePosition - _GameObject->GetLocation();
	m_angle = std::atan2(delta.y, delta.x);


	//Calculate position of firing point
	AG::Vector2<float>  orbitPosition;
	orbitPosition.x = _GameObject->GetLocation().x + m_orbitRadius * std::cos(m_angle);
	orbitPosition.y = _GameObject->GetLocation().y + m_orbitRadius * std::sin(m_angle);

	//Set poisition based of the orbit
	m_FiringPoint.setPosition(orbitPosition);

	//std::cout << "Firing Point Location: (" << m_FiringPoint.getPosition().x << ", " << m_FiringPoint.getPosition().y << ")" << std::endl;

	//Draw the rotating rectangle here - not adding it to the gameobject vector as a seprate gameObject so just draw it here. 
	window.draw(m_FiringPoint);
}

//Get fire point location
const AG::Vector2<float> &PlayerComponent::GetFirePointLocation()
{
	m_FiringPointLocation = AG::Vector2<float>(m_FiringPoint.getPosition().x, m_FiringPoint.getPosition().y);
	return m_FiringPointLocation;
}

//Get mouse position
AG::Vector2<float>& PlayerComponent::GetMousePosition()
{
	return m_MousePosition;
}

//The direction the bullet will travel in
AG::Vector2<float> PlayerComponent::CalculateDirection()
{
	AG::Vector2<float> direction = GetMousePosition() - GetFirePointLocation();
	return direction;
}

void PlayerComponent::Shooting()
{
	if (m_timeSinceLastShot >= 1.25f && _GameObject->GetIsShooting())
	{

		//Create a new bullet
		std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>(GetFirePointLocation(), 75.0f, CalculateDirection());

		//Put bullet on the list
		GameManager::AddBulletObjectList(std::move(newBullet));
		ResourceManager::PlaySound("Shoot");

		m_timeSinceLastShot = 0.0f;
	}
}

#pragma endregion
