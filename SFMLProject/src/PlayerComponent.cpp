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



	//Set texture to be the whole sprite sheet
	m_PlayerShapeRectangle.setTexture(&ResourceManager::GetTexture("Player"));


	//Set the texture to the cookie cutter section of the sprite sheet
	m_PlayerShapeRectangle.setTextureRect(m_PlayerTextureUV);


	//Set Rect to the middle / position
	m_PlayerShapeRectangle.setOrigin(m_PlayerSize / 2);
	m_PlayerShapeRectangle.setPosition(_GameObject->GetLocation());



	CreateFiringPoint();

	//Actually set it
	_GameObject->SetRectangleShape(m_PlayerShapeRectangle);




	//const sf::IntRect* textureRectPtr = &_GameObject->GetRectangleShape().getTextureRect();
	//std::cout << "Texture Rect Address: " << static_cast<const void*>(textureRectPtr) << std::endl;
	// 
	//sf::IntRect textureRect = _GameObject->GetTextureRect();
	//std::cout << "Texture Rect: left=" << textureRect.left << ", top=" << textureRect.top << ", width=" << textureRect.width << ", height=" << textureRect.height << std::endl;

}

PlayerComponent::~PlayerComponent()
{

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
		_GameObject->GetLocation().x -= m_MovementSpeed * deltaTime;


		animationComponent->SetPlayerAnimation(AnimationComponent::PlayerStates::Left);
		m_PlayerShapeRectangle.setTextureRect(animationComponent->GetCurrentFrame(animationComponent->GetPlayerState(), deltaTime, animationComponent->GetPlayerAnimationsMap()));
		_GameObject->SetRectangleShape(m_PlayerShapeRectangle);

		direction.x = -1.0f;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		_GameObject->GetLocation().x += m_MovementSpeed * deltaTime;


		animationComponent->SetPlayerAnimation(AnimationComponent::PlayerStates::Right);
		m_PlayerShapeRectangle.setTextureRect(animationComponent->GetCurrentFrame(animationComponent->GetPlayerState(), deltaTime, animationComponent->GetPlayerAnimationsMap()));
		_GameObject->SetRectangleShape(m_PlayerShapeRectangle);


		direction.x = 1.0f;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		_GameObject->GetLocation().y -= m_MovementSpeed * deltaTime;


		animationComponent->SetPlayerAnimation(AnimationComponent::PlayerStates::Up);
		m_PlayerShapeRectangle.setTextureRect(animationComponent->GetCurrentFrame(animationComponent->GetPlayerState(), deltaTime, animationComponent->GetPlayerAnimationsMap()));
		_GameObject->SetRectangleShape(m_PlayerShapeRectangle);


		direction.y = -1.0f;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		_GameObject->GetLocation().y += m_MovementSpeed * deltaTime;


		animationComponent->SetPlayerAnimation(AnimationComponent::PlayerStates::Down);
		m_PlayerShapeRectangle.setTextureRect(animationComponent->GetCurrentFrame(animationComponent->GetPlayerState(), deltaTime, animationComponent->GetPlayerAnimationsMap()));
		_GameObject->SetRectangleShape(m_PlayerShapeRectangle);


		direction.y = 1.0f;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		//GameManager::RestartGame();
	}
	else
	{
		animationComponent->SetPlayerAnimation(AnimationComponent::PlayerStates::Idle);
		m_PlayerShapeRectangle.setTextureRect(animationComponent->GetCurrentFrame(animationComponent->GetPlayerState(), deltaTime, animationComponent->GetPlayerAnimationsMap()));
		_GameObject->SetRectangleShape(m_PlayerShapeRectangle);
	}


	direction.Normalise();

	_GameObject->GetMoveDirection() = direction;

	_GameObject->GetRectangleShape().setPosition(_GameObject->GetLocation());
}


#pragma region ShooingAndAiming

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
	//Also avoid any collision detections because its a rectangle shape that would be on the vector
	window.draw(m_FiringPoint);
}

const AG::Vector2<float> &PlayerComponent::GetFirePointLocation()
{
	m_FiringPointLocation = AG::Vector2<float>(m_FiringPoint.getPosition().x, m_FiringPoint.getPosition().y);
	return m_FiringPointLocation;
}

float &PlayerComponent::GetFiringPointRotation()
{
	return m_angle;
}

AG::Vector2<float>& PlayerComponent::GetMousePosition()
{
	return m_MousePosition;
}

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
		//ResourceManager::PlaySound(ResourceManager::GetSoundBuffer("Shoot"));

		m_timeSinceLastShot = 0.0f;

		
	}
}

#pragma endregion
