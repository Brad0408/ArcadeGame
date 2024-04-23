#include <iostream>
#include <chrono>
#include <Vector2.h>
#include <Event.h>
#include <GameObject.h>

#define FIXEDFRAMERATE 0.025f



int main()
{
	//Vector that stores all the created gameObjects
	std::vector<GameObject*> gameObjects;


	GameObject* Player = new GameObject();
	GameObject* Wall = new GameObject();

	//Put newly made gameObjects on the vector
	gameObjects.push_back(Player);
	gameObjects.push_back(Wall);


	Player->SetName("Player");
	Wall->SetName("Wall");
	for (GameObject* object : gameObjects)
	{
		std::cout << object->GetName() << std::endl;
	}


	Player->AddComponent<PlayerComponent>();
	Player->AddComponent<BoxCollider>();

	Player->GetComponent<BoxCollider>()->DrawOutlines(Player->GetRectangleShape());
	Wall->AddComponent<BoxCollider>();




	//Create window of resolution
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML Works");

#pragma region TestSqaure

	//Define the size of rectangle
	AG::Vector2<float> BoxSize = AG::Vector2<float>::one * 100;
	AG::Vector2<float> BoxPos = AG::Vector2<float>(350.0f, 400.0f);;

	sf::IntRect altrectTextureUV(35, 0, 24, 24);

	//Create a rectangle
	sf::RectangleShape Boxshape(BoxSize);

	//Set texture to be the whole sprite sheet
	Boxshape.setTexture(&ResourceManager::GetTexture("Player"));

	//Set the texture to the cookie cutter section of the sprite sheet
	Boxshape.setTextureRect(altrectTextureUV);

	Boxshape.setPosition(BoxPos);

	//Boxshape.setOutlineThickness(3);
	//Boxshape.setOutlineColor(sf::Color(255, 255, 255));

	Wall->SetRectangleShape(Boxshape);


	Wall->GetComponent<BoxCollider>()->DrawOutlines(Wall->GetRectangleShape());

#pragma endregion




	//Get the time before getting into the loop
	std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
	float deltaTime = 0.0f;
	float timeSincePhysicsStep = 0.0f;


	//Test shape thats squished
	sf::RectangleShape TestShape(sf::Vector2f(100, 50));
	TestShape.setFillColor(sf::Color::Green);
	TestShape.setPosition(200, 200);

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



		//Actually add the shape to the screen
		window.draw(Player->GetRectangleShape());
		window.draw(Wall->GetRectangleShape());
		window.draw(TestShape);

		//Display whats actually been rendered
		window.display();


		Player->GetComponent<PlayerComponent>()->Move();


		//Check for collisions between game objects
		for (size_t i = 0; i < gameObjects.size(); ++i) 
		{
			for (size_t j = i + 1; j < gameObjects.size(); ++j) 
			{
				if (gameObjects[i]->HasBoxCollider(gameObjects[i]) && gameObjects[j]->HasBoxCollider(gameObjects[j]))
				{
					BoxCollider* colliderA = gameObjects[i]->GetComponent<BoxCollider>();
					BoxCollider* colliderB = gameObjects[j]->GetComponent<BoxCollider>();

					sf::FloatRect boundsA = gameObjects[i]->GetRectangleShape().getGlobalBounds();
					sf::FloatRect boundsB = gameObjects[j]->GetRectangleShape().getGlobalBounds();

					colliderA->SetBounds(boundsA);
					colliderA->SetBounds(boundsB);

					if (colliderA->CheckCollision(*colliderB)) 
					{
						std::cout << "Collision" << std::endl;
					}
				}
			}
		}

		






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
