#include <iostream>
#include <chrono>
#include <Vector2.h>
#include <Event.h>
#include <GameObject.h>

#define FIXEDFRAMERATE 0.025f




int main()
{



	GameObject* Player = new GameObject();
	GameObject* Enemy = new GameObject();
	std::array<GameObject*, 4> walls;

	

	for (int i = 0; i < 4; ++i)
	{
		walls[i] = new GameObject();
	}

	//Put newly made gameObjects on the vector
	GameManager::AddGameObject(Player);
	GameManager::AddGameObject(Enemy);


	for (int i = 0; i < 4; ++i) 
	{
		GameManager::AddGameObject(walls[i]);
	}



	Player->SetName("Player");
	Enemy->SetName("Enemy");
	std::array<std::string, 4> wallNames = { "TopWall", "LeftWall", "BottomWall", "RightWall" };

	for (int i = 0; i < walls.size(); ++i) 
	{
		walls[i]->SetName(wallNames[i]);
	}


	//Out the names of the store gameobejcts to check they exist on the vector
	GameManager::GetGameObjectNames(GameManager::GetGameObjectVector());


	Player->AddComponent<PlayerComponent>();
	Player->AddComponent<BoxCollider>();
	Player->GetComponent<BoxCollider>()->DrawOutlines(Player->GetRectangleShape());


	Enemy->AddComponent<BoxCollider>();
	Enemy->AddComponent<EnemyComponent>();
	Enemy->GetComponent<BoxCollider>()->DrawOutlines(Enemy->GetRectangleShape());

	for (int i = 0; i < 4; ++i) 
	{
		walls[i]->AddComponent<BoxCollider>();
	}


	//Create window of resolution
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML Works");



#pragma region Walls

	float TopPadding = 20.0f;
	float LeftPadding = 20.0f;
	float BottomPadding = 960.0f;
	float RightPadding = 960.0f;

	//Define the size and locations of the wall
	AG::Vector2<float> VerticalWallSize = AG::Vector2<float>(15, 1000);
	AG::Vector2<float> HorizontalWallSize = AG::Vector2<float>(1000, 15);

	AG::Vector2<float> TopWallPos = AG::Vector2<float>(0.0f, TopPadding);
	AG::Vector2<float> LeftWallPos = AG::Vector2<float>(LeftPadding, 0.0f);
	AG::Vector2<float> BottomWallPos = AG::Vector2<float>(0.0f, BottomPadding);
	AG::Vector2<float> RightWallPos = AG::Vector2<float>(RightPadding, 0.0f);

	//Create a rectangle
	sf::RectangleShape TopWallShape(HorizontalWallSize);
	sf::RectangleShape LeftWallShape(VerticalWallSize);
	sf::RectangleShape BottomWallShape(HorizontalWallSize);
	sf::RectangleShape RightWallShape(VerticalWallSize);

	TopWallShape.setFillColor(sf::Color::Green);
	LeftWallShape.setFillColor(sf::Color::Blue);
	BottomWallShape.setFillColor(sf::Color::Magenta);
	RightWallShape.setFillColor(sf::Color::Yellow);

	TopWallShape.setPosition(TopWallPos);
	LeftWallShape.setPosition(LeftWallPos);
	BottomWallShape.setPosition(BottomWallPos);
	RightWallShape.setPosition(RightWallPos);


	TopWallShape.setOutlineThickness(3);
	LeftWallShape.setOutlineThickness(3);
	BottomWallShape.setOutlineThickness(3);
	RightWallShape.setOutlineThickness(3);

	TopWallShape.setOutlineColor(sf::Color(255, 255, 255));
	LeftWallShape.setOutlineColor(sf::Color(255, 255, 255));
	BottomWallShape.setOutlineColor(sf::Color(255, 255, 255));
	RightWallShape.setOutlineColor(sf::Color(255, 255, 255));

	walls[0]->SetRectangleShape(TopWallShape);
	walls[1]->SetRectangleShape(LeftWallShape);
	walls[2]->SetRectangleShape(BottomWallShape);
	walls[3]->SetRectangleShape(RightWallShape);


	for (int i = 0; i < 4; ++i)
	{
		walls[i]->SetIsWall(true);
	}

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

			// Check for left mouse button press event
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
				Player->SetIsShooting(true);
			}

			// Check for left mouse button release event
			if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
			{
				Player->SetIsShooting(false);
			}
		}

		std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
		deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - lastTime).count() / 100000.0f;
		lastTime = now;

		Player->GetComponent<PlayerComponent>()->Move(deltaTime);

		if (Player->GetIsShooting())
		{
			Player->GetComponent<PlayerComponent>()->Shooting();
		}


		Enemy->GetComponent<EnemyComponent>()->Move();


		//Retrieve the vector of GameObjects from the GameManager
		std::vector<GameObject*> &gameObjects = GameManager::GetGameObjectVector();

		//Check for collisions between game objects
		for (int i = 0; i < gameObjects.size(); ++i)
		{
			for (int j = i + 1; j < gameObjects.size(); ++j)
			{
				GameObject* objectA = gameObjects[i];
				GameObject* objectB = gameObjects[j];

				//Skip collision checks if both objects are walls
				if (objectA->GetIsWall() && objectB->GetIsWall())
				{
					continue;
				}


				if (objectA->HasBoxCollider(objectA) && objectB->HasBoxCollider(objectB))
				{
					BoxCollider* colliderA = objectA->GetComponent<BoxCollider>();
					BoxCollider* colliderB = objectB->GetComponent<BoxCollider>();

					// Ensure both colliders are valid before checking collision
					if (colliderA && colliderB && colliderA->CheckCollision(objectA, objectB))
					{
						//Wall Detection
						if (objectA->GetIsWall() || objectB->GetIsWall())
						{
							std::cout << "Wall Collision deteced between objects " << objectA->GetName() << " and " << objectB->GetName() << std::endl;


							for (GameObject* wall : walls)
							{
								BoxCollider::WallCollision(objectA, objectB);
							}

						}
						//Detection between none walls
						else
						{
							std::cout << "Collision detected between objects " << objectA->GetName() << " and " << objectB->GetName() << std::endl;
						}
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

		//Clear
		window.clear();

		//Loop through each GameObject in the vector
		for (GameObject* gameObject : gameObjects) 
		{
			window.draw(gameObject->GetRectangleShape());
		}

		Player->GetComponent<PlayerComponent>()->CalculateFiringPointRotation(window);
		
		//window.draw(TestShape);
		
		//Display whats actually been rendered
		window.display();

	}

	//Memory Cleanup
	ResourceManager::ClearTextureMap();
	GameManager::ClearGameObjectVector();

	return 0;
}
