#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <Vector2.h>

#define FIXEDFRAMERATE 0.025f

int main()
{
	//Create window of resolution
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML Works");

	//Define the size of rect
	AG::Vector2<float> rectSize = AG::Vector2<float>::one * 100;

	//Define the postion of the rect
	AG::Vector2<float> rectPos = AG::Vector2<float>(900, 450);

	//Load the texture
	sf::Texture rectTex; rectTex.loadFromFile("Textures/robotronsprites.jpg");
	AG::Vector2i spritesInTex(21, 10);
	AG::Vector2i rectTextSize(rectTex.getSize());
	sf::IntRect rectTexUV(0, (rectTextSize.y / spritesInTex.y) * 5, rectTextSize.x / spritesInTex.x, rectTextSize.y / spritesInTex.y);

	//Create a rectangle
	sf::RectangleShape shape(rectSize);

	//Set texture in rectangle
	shape.setTexture(&rectTex);
	shape.setTextureRect(rectTexUV);

	//Set Rect to the middle
	shape.setOrigin(rectSize / 2);

	//Set the postion of the rect
	shape.setPosition(rectPos);


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


			//if (event.type == sf::Event::MouseButtonPressed)
			//{
			//	shape.setFillColor(sf::Color::Blue);
			//}
			//else if (event.type == sf::Event::MouseButtonReleased)
			//{
			//	shape.setFillColor(sf::Color::Green);
			//}

		}

		std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
		deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - lastTime).count() / 100000.0f;
		lastTime = now;


		//Clear
		window.clear();

		//Actually add the circle to the screen
		window.draw(shape);

		//Display whats actually been rendered
		window.display();


		//std::cout << "Delta Time: " << deltaTime << " seconds" << std::endl;
		
		timeSincePhysicsStep += deltaTime;
		while (timeSincePhysicsStep > FIXEDFRAMERATE)
		{

			std::cout << "timeSincePhysicsStep: " << timeSincePhysicsStep << std::endl;


			timeSincePhysicsStep -= FIXEDFRAMERATE;
		}

		

	}

	return 0;
}


