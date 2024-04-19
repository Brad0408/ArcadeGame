#include "PlayerComponent.h"

PlayerComponent::PlayerComponent(GameObject* owner) : Component(owner)
{
	
	std::cout << "PlayerCompont Constructed" << std::endl;
	std::cout << "GameObject value in PlayerComponent : " << _GameObject << std::endl;

	_GameObject->SetLocation(650, 450);


	sf::Texture WholeSpriteSheet;
	WholeSpriteSheet.loadFromFile("Textures/robotronsprites.jpg");

	if (!WholeSpriteSheet.loadFromFile("Textures/robotronsprites.jpg"))
	{
		std::cout << "Failed to Load Texture" << std::endl;
	}
	else
	{
		std::cout << "Texture Found" << std::endl;
	}

	

	//Cookie cutter part of sprite sheet
	sf::IntRect PlayerTextureUV(0, 0, 24, 24);

	PlayerShapeRectangle.setSize(PlayerSize);


	//PlayerShapeRectangle.setFillColor(sf::Color::Green);



	//Set texture to be the whole sprite sheet
	//PlayerShapeRectangle.setTexture(&WholeSpriteSheet);


	//Set the texture to the cookie cutter section of the sprite sheet
	//PlayerShapeRectangle.setTextureRect(PlayerTextureUV);




	



	//Set Rect to the middle / position
	PlayerShapeRectangle.setOrigin(PlayerSize / 2);
	PlayerShapeRectangle.setPosition(_GameObject->GetLocation());


	
	//Set textures within the gameObject
	_GameObject->SetTexture(&WholeSpriteSheet);
	_GameObject->SetTextureRect(PlayerTextureUV);

	//Get the gameobject texture values and set it back to this local PlayerShape
	PlayerShapeRectangle.setTexture(&_GameObject->GetTexture());
	PlayerShapeRectangle.setTextureRect(_GameObject->GetTextureRect());


	_GameObject->DrawOutlines(PlayerShapeRectangle);



	//Actually set it
	_GameObject->SetRectangleShape(PlayerShapeRectangle);


	const sf::IntRect* textureRectPtr = &_GameObject->GetRectangleShape().getTextureRect();
	std::cout << "Texture Rect Address: " << static_cast<const void*>(textureRectPtr) << std::endl;

	sf::IntRect textureRect = _GameObject->GetTextureRect();

	std::cout << "Texture Rect: left=" << textureRect.left << ", top=" << textureRect.top << ", width=" << textureRect.width << ", height=" << textureRect.height << std::endl;

}
