#include <iostream>
#include <chrono>
#include <Vector2.h>
#include <Event.h>
#include <GameObject.h>

#define FIXEDFRAMERATE 0.020f



int main()
{
	//Load the texture
	sf::Texture rectTexture;
	rectTexture.loadFromFile("Textures/robotronsprites.jpg");


	GameObject* Player = new GameObject();

	Player->AddComponent<PlayerComponent>();

	sf::Texture testTex = Player->GetTexture();

	//Player->SetTexture(&rectTexture);



	//Create window of resolution
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML Works");

	//Define the size of rectangle
	AG::Vector2<float> BoxSize = AG::Vector2<float>::one * 100;
	AG::Vector2<float> BoxPos = AG::Vector2<float>(350.0f, 400.0f);;









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



		Player->GetComponent<PlayerComponent>()->Move();

		
		timeSincePhysicsStep += deltaTime;
		while (timeSincePhysicsStep > FIXEDFRAMERATE)
		{

	
			
			//std::cout << "timeSincePhysicsStep: " << timeSincePhysicsStep << std::endl;
			//std::cout << "Player Location: (" << Player->location.x << ", " << Player->location.y << ")" << std::endl;



			timeSincePhysicsStep -= FIXEDFRAMERATE;
		}
	}

	return 0;
}
