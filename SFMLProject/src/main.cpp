#include <iostream>
#include <chrono>
#include <Vector2.h>
#include <Event.h>
#include <GameObject.h>

#define FIXEDFRAMERATE 0.020f



int main()
{

	GameObject* Player = new GameObject();

	std::cout << "Player value " << Player << std::endl;





	Player->AddComponent<PlayerComponent>();


	//Create window of resolution
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML Works");

	//Define the size of rectangle
	AG::Vector2<float> BoxSize = AG::Vector2<float>::one * 100;


	AG::Vector2<float> TestBoxLocation(350.0f, 400.0f);
	AG::Vector2<float> BoxPos = TestBoxLocation;


	//Load the texture
	sf::Texture rectTexture; 
	rectTexture.loadFromFile("Textures/robotronsprites.jpg");



	//const int rows = 10;
	//const int colums = 21;

	//Number of sprites in the texture 
	//AG::Vector2i spritesInSpriteSheet(colums, rows);

	//Returns size of texture (608x512)
	//AG::Vector2i rectTextureSize(rectTexture.getSize());

										         // 512/10 * 5 = 256							// 608/21 = 28.9							// 512/10 = 51.2
	//sf::IntRect rectTextureUV(0, (rectTextureSize.y / spritesInSpriteSheet.y) * 5, rectTextureSize.x / spritesInSpriteSheet.x, rectTextureSize.y / spritesInSpriteSheet.y);

	// (0,256 ) are the coordinates on the texture/sprite sheet
	//(28.9, 51.2 ) is the size of how much you want to cookie cutter out of the sprite sheet


	sf::IntRect altrectTextureUV(35, 0, 24, 24);

	//Create a rectangle
	sf::RectangleShape Boxshape(BoxSize);

	//Set texture to be the whole sprite sheet
	Boxshape.setTexture(&rectTexture);

	//Set the texture to the cookie cutter section of the sprite sheet
	Boxshape.setTextureRect(altrectTextureUV);

	Boxshape.setPosition(BoxPos);

	Boxshape.setOutlineThickness(3);
	Boxshape.setOutlineColor(sf::Color(255, 255, 255));

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
			//	shape.setTextureRect(altrectTextureUV);
			//}
			//else if (event.type == sf::Event::MouseButtonReleased)
			//{
			//	shape.setTextureRect(rectTextureUV);
			//}	
		}

		std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
		deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - lastTime).count() / 100000.0f;
		lastTime = now;


		//Clear
		window.clear();

		//Test shape thats squished
		sf::RectangleShape TestShape(sf::Vector2f(100, 50));
		TestShape.setFillColor(sf::Color::Green);
		TestShape.setPosition(200, 200);



		//Actually add the shape to the screen
		window.draw(Player->GetRectangleShape());
		window.draw(Boxshape);
		window.draw(TestShape);

		//Display whats actually been rendered
		window.display();

		

		
		timeSincePhysicsStep += deltaTime;
		while (timeSincePhysicsStep > FIXEDFRAMERATE)
		{

			//std::cout << "timeSincePhysicsStep: " << timeSincePhysicsStep << std::endl;
			//std::cout << "Player Location: (" << Player->location.x << ", " << Player->location.y << ")" << std::endl;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				Player->GetLocation().x -= 1.0f;

				Player->GetRectangleShape().setPosition(Player->GetLocation());


			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				Player->GetLocation().x += 1.0f;

				Player->GetRectangleShape().setPosition(Player->GetLocation());
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				Player->GetLocation().y -= 1.0f;

				Player->GetRectangleShape().setPosition(Player->GetLocation());
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				Player->GetLocation().y += 1.0f;

				Player->GetRectangleShape().setPosition(Player->GetLocation());
			}

			timeSincePhysicsStep -= FIXEDFRAMERATE;
		}
	}

	return 0;
}


