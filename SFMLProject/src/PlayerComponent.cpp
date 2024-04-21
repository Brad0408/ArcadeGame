#include "PlayerComponent.h"

PlayerComponent::PlayerComponent(GameObject* owner) : Component(owner)
{
	
	//std::cout << "PlayerCompont Constructed" << std::endl;
	//std::cout << "GameObject value in PlayerComponent : " << _GameObject << std::endl;

	_GameObject->SetLocation(650, 450);


	
	


	//Cookie cutter part of sprite sheet (0,0 = Coordinates, 24, 24 = Size of rectangle)
	sf::IntRect PlayerTextureUV(210, 164, 24, 24);

	PlayerShapeRectangle.setSize(PlayerSize);



	//Set texture to be the whole sprite sheet
	PlayerShapeRectangle.setTexture(&ResourceManager::GetTexture("Player"));


	//Set the texture to the cookie cutter section of the sprite sheet
	PlayerShapeRectangle.setTextureRect(PlayerTextureUV);



	//Set Rect to the middle / position
	PlayerShapeRectangle.setOrigin(PlayerSize / 2);
	PlayerShapeRectangle.setPosition(_GameObject->GetLocation());


	_GameObject->DrawOutlines(PlayerShapeRectangle);


	//Actually set it
	_GameObject->SetRectangleShape(PlayerShapeRectangle);

	_GameObject->AddComponent<BoxCollider>();



	//const sf::IntRect* textureRectPtr = &_GameObject->GetRectangleShape().getTextureRect();
	//std::cout << "Texture Rect Address: " << static_cast<const void*>(textureRectPtr) << std::endl;
	// 
	//sf::IntRect textureRect = _GameObject->GetTextureRect();
	//std::cout << "Texture Rect: left=" << textureRect.left << ", top=" << textureRect.top << ", width=" << textureRect.width << ", height=" << textureRect.height << std::endl;

}


void PlayerComponent::Move()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		_GameObject->GetLocation().x -= m_MovementSpeed;

		_GameObject->GetRectangleShape().setPosition(_GameObject->GetLocation());
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		_GameObject->GetLocation().x += m_MovementSpeed;

		_GameObject->GetRectangleShape().setPosition(_GameObject->GetLocation());
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		_GameObject->GetLocation().y -= m_MovementSpeed;

		_GameObject->GetRectangleShape().setPosition(_GameObject->GetLocation());
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		_GameObject->GetLocation().y += m_MovementSpeed;

		_GameObject->GetRectangleShape().setPosition(_GameObject->GetLocation());
	}
}
