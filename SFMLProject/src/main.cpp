#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <Vector2.h>
#include <Event.h>
#include <GameObject.h>


#define FIXEDFRAMERATE 0.020f



int main()
{

	GameObject* Player = new GameObject();

	std::cout << Player << std::endl;

	AG::Vector2<float> PlayerLocation(900.0f, 450.0f);







	//Create window of resolution
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML Works");

	//Define the size of rectangle
	AG::Vector2<float> rectSize = AG::Vector2<float>::one * 100;

	//Define the postion of the rectangle
	AG::Vector2<float> rectPos = PlayerLocation;

	//Load the texture
	sf::Texture rectTexture; 
	rectTexture.loadFromFile("Textures/robotronsprites.jpg");


	const int rows = 10;
	const int colums = 21;

	//Number of sprites in the texture 
	AG::Vector2i spritesInSpriteSheet(colums, rows);

	//Returns size of texture (608x512)
	AG::Vector2i rectTextureSize(rectTexture.getSize());



										         // 512/10 * 5 = 256							// 608/21 = 28.9							// 512/10 = 51.2
	//sf::IntRect rectTextureUV(0, (rectTextureSize.y / spritesInSpriteSheet.y) * 5, rectTextureSize.x / spritesInSpriteSheet.x, rectTextureSize.y / spritesInSpriteSheet.y);
	sf::IntRect rectTextureUV(0, 0, 24, 24);
	sf::IntRect altrectTextureUV(35, 0, 24, 24);

	// (0,256 ) are the coordinates on the texture/sprite sheet
	//(28.9, 51.2 ) is the size of how much you want to cookie cutter out of the sprite sheet

	//Create a rectangle
	sf::RectangleShape shape(rectSize);


	//Set texture to be the whole sprite sheet
	shape.setTexture(&rectTexture);

	//Set the texture to the cookie cutter section of the sprite sheet
	shape.setTextureRect(rectTextureUV);

	//Set Rect to the middle
	shape.setOrigin(rectSize / 2);

	//Set the postion of the rect
	shape.setPosition(rectPos);

	shape.setOutlineThickness(3);
	shape.setOutlineColor(sf::Color(255, 255, 255));


	//Get the time before getting the 
	std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
	float deltaTime = 0.0f;
	float timeSincePhysicsStep = 0.0f;


	while (window.isOpen())
	{
		//Declare event
		sf::Event event;

		while (window.pollEvent(event))
		{
			//if the event type if closing the window - close the window
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}


			if (event.type == sf::Event::MouseButtonPressed)
			{
				shape.setTextureRect(altrectTextureUV);
			}
			else if (event.type == sf::Event::MouseButtonReleased)
			{
				shape.setTextureRect(rectTextureUV);
			}



			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				//std::cout << "A pressed" << std::endl;
				//std::cout << PlayerLocation.x << std::endl;

				PlayerLocation.x -= 10.0f;

				shape.setPosition(PlayerLocation);

				
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				PlayerLocation.x += 10.0f;

				shape.setPosition(PlayerLocation);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				PlayerLocation.y -= 10.0f;

				shape.setPosition(PlayerLocation);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				PlayerLocation.y += 10.0f;

				shape.setPosition(PlayerLocation);
			}

		}

		std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
		deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - lastTime).count() / 100000.0f;
		lastTime = now;


		//Clear
		window.clear();

		//Actually add the shape to the screen
		window.draw(shape);

		//Display whats actually been rendered
		window.display();

		



		//std::cout << "Delta Time: " << deltaTime << " seconds" << std::endl;
		
		timeSincePhysicsStep += deltaTime;
		while (timeSincePhysicsStep > FIXEDFRAMERATE)
		{

			//std::cout << "timeSincePhysicsStep: " << timeSincePhysicsStep << std::endl;


			timeSincePhysicsStep -= FIXEDFRAMERATE;
		}
	}

	return 0;
}


