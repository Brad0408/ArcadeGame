#include "PlayerComponent.h"
#include "Bullet.h"


PlayerComponent::PlayerComponent(GameObject* owner) : Component(owner)
{
	
	//std::cout << "PlayerCompont Constructed" << std::endl;
	//std::cout << "GameObject value in PlayerComponent : " << _GameObject << std::endl;

	_GameObject->SetLocation(650, 450);


	
	


	//Cookie cutter part of sprite sheet (0,0 = Coordinates, 24, 24 = Size of rectangle)
	m_PlayerTextureUV = sf::IntRect(210, 164, 24, 24);

	m_PlayerShapeRectangle.setSize(m_PlayerSize);



	//Set texture to be the whole sprite sheet
	m_PlayerShapeRectangle.setTexture(&ResourceManager::GetTexture("Player"));


	//Set the texture to the cookie cutter section of the sprite sheet
	m_PlayerShapeRectangle.setTextureRect(m_PlayerTextureUV);


	//Set Rect to the middle / position
	m_PlayerShapeRectangle.setOrigin(m_PlayerSize / 2);
	m_PlayerShapeRectangle.setPosition(_GameObject->GetLocation());


	//_GameObject->DrawOutlines(PlayerShapeRectangle);

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


void PlayerComponent::Move(float deltaTime)
{
	AG::Vector2<float> direction(0.0f, 0.0f);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		_GameObject->GetLocation().x -= m_MovementSpeed * deltaTime;

		direction.x = -1.0f;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		_GameObject->GetLocation().x += m_MovementSpeed * deltaTime;

		direction.x = 1.0f;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		_GameObject->GetLocation().y -= m_MovementSpeed * deltaTime;

		direction.y = -1.0f;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		_GameObject->GetLocation().y += m_MovementSpeed * deltaTime;

		direction.y = 1.0f;
	}

	direction.Normalise();

	_GameObject->GetMoveDirection() = direction;

	_GameObject->GetRectangleShape().setPosition(_GameObject->GetLocation());
}



void PlayerComponent::CreateFiringPoint()
{
	m_FiringPoint.setSize(m_FiringPointSize);
	m_FiringPoint.setFillColor(sf::Color::Red);
	m_FiringPoint.setOrigin(m_FiringPoint.getSize().x / 2.0f, m_FiringPoint.getSize().y / 2.0f);

	std::cout << "firing poi9nt created" << std::endl;
}

void PlayerComponent::CalculateFiringPointRotation(sf::RenderWindow &window)
{
	//Calculate angle between player and mouse cursor
	AG::Vector2<float>  mousePosition = AG::Vector2<float>(sf::Mouse::getPosition(window));
	AG::Vector2<float>  delta = mousePosition - _GameObject->GetLocation();
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

AG::Vector2<float>& PlayerComponent::GetFirePointLocation()
{
	return m_FiringPoint.getGlobalBounds();
}

void PlayerComponent::Shooting(sf::RenderWindow& window)
{
	Bullet newBullet;

	newBullet.SetLocation(500,500);

	std::cout << newBullet.GetLocation().x << " , " << newBullet.GetLocation().y << std::endl;

	window.draw(newBullet.GetCircleShape());
}
