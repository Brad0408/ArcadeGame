#include <iostream>
#include <chrono>
#include <Vector2.h>
#include <Event.h>
#include <GameObject.h>
#include <Bullet.h>

#define FIXEDFRAMERATE 0.025f




int main()
{

	GameManager::CreatePlayer();
	GameManager::CreateEnemyPool(10);

	std::array<GameObject*, 4> walls;


	for (int i = 0; i < 4; ++i)
	{
		walls[i] = new GameObject();
	}

	//Put newly made gameObjects on the List
	GameManager::AddGameObjectList(GameManager::GetEnemyList());



	for (int i = 0; i < 4; ++i) 
	{
		GameManager::AddGameObjectList(walls[i]);
	}

	std::array<std::string, 4> wallNames = { "TopWall", "LeftWall", "BottomWall", "RightWall" };

	for (int i = 0; i < walls.size(); ++i) 
	{
		walls[i]->SetName(wallNames[i]);
	}


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
	//sf::RectangleShape TestShape(sf::Vector2f(100, 50));
	//TestShape.setFillColor(sf::Color::Green);
	//TestShape.setPosition(200, 200);
	




	std::list<std::unique_ptr<GameObject>>& gameObjectsL = GameManager::GetGameObjectList();
	std::list<std::unique_ptr<Bullet>>& bulletObjectsL = GameManager::GetBulletsList();
	GameManager::GetGameObjectListsNames(GameManager::GetGameObjectList());

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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



		//Check for collisions between game objects
		for (auto it = gameObjectsL.begin(); it != gameObjectsL.end(); ++it)
		{
			for (auto jt = std::next(it); jt != gameObjectsL.end(); ++jt)
			{
				GameObject* objectA = it->get(); // Dereference the shared pointer to get the GameObject pointer
				GameObject* objectB = jt->get(); // Dereference the shared pointer to get the GameObject pointer



				//Skip collision checks if both objects are walls
				if (objectA->GetIsWall() && objectB->GetIsWall())
				{
					continue;
				}

				if (objectA->GetIsEnemy() && objectB->GetIsEnemy())
				{
					continue;
				}


		
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
						objectA->MarkForRemoval();
						objectB->MarkForRemoval();
						
					

						float CreationTime = 0.0f;
						bool playerCreationStarted = false;
						CreationTime += deltaTime;
				

						if (!playerCreationStarted && CreationTime >= 0.0001f)
						{
							GameManager::CreatePlayer();
							playerCreationStarted = true;
							std::cout << "PlayerCreated " << std::endl;
						}


						std::cout << "Collision detected between objects " << objectA->GetName() << " and " << objectB->GetName() << std::endl;
					}
				}
			}
		}




	
		//Check for collisions between bullets and other objects
		for (auto bulletIt = bulletObjectsL.begin(); bulletIt != bulletObjectsL.end(); ++bulletIt)
		{
			for (auto gameObjectIt = gameObjectsL.begin(); gameObjectIt != gameObjectsL.end(); ++gameObjectIt)
			{
				Bullet* bullet = bulletIt->get(); // Dereference the shared pointer to get the Bullet pointer
				GameObject* gameObject = gameObjectIt->get(); // Dereference the shared pointer to get the GameObject pointer


				//Skip collision checks if both object is a player
				if (gameObject->GetIsPlayer())
				{
					continue;
				}


				// Check collision between bullet and other game objects
				CircleCollider* bulletCollider = bullet->GetComponent<CircleCollider>();
				BoxCollider* gameObjectCollider = gameObject->GetComponent<BoxCollider>();


				// Ensure both colliders are valid before checking collision
				if (bulletCollider && gameObjectCollider && bulletCollider->BulletCollision(bullet, gameObject))
				{

					// Check if the object is a wall
					if (gameObject->GetIsWall())
					{
						bullet->MarkForRemoval();
						std::cout << "Collision detected between bullet " << bullet << " and " << gameObject->GetName() << std::endl;
		
					}
					else
					{
						gameObject->MarkForRemoval();
						bullet->MarkForRemoval();

				



						std::cout << "Collision detected between bullet " << bullet << " and " << gameObject->GetName() << std::endl;
					
					}
		
				}
			}
		}





		bool enemiesExist = false;

		// Iterate over the game objects to check for enemies
		for (const auto& gameObject : gameObjectsL)
		{
			if (gameObject->GetTag() == "Enemy")
			{
				// At least one enemy is found
				enemiesExist = true;
				break;
			}
		}

		// Check if no enemies are found
		if (!enemiesExist)
		{
			// Call CreateEnemyPool function if no enemies are found
			GameManager::CreateEnemyPool(10);
			GameManager::AddGameObjectList(GameManager::GetEnemyList());
		}

		






		timeSincePhysicsStep += deltaTime;
		while (timeSincePhysicsStep > FIXEDFRAMERATE)
		{
			timeSincePhysicsStep -= FIXEDFRAMERATE;
		}

		//Clear
		window.clear();


		/////////////Rendering////////////////

		for (const auto& gameObjectPtr : gameObjectsL)
		{
			window.draw(gameObjectPtr->GetRectangleShape());
		}

		for (const auto& bullObjectPtr : bulletObjectsL)
		{
			window.draw(bullObjectPtr->GetCircleShape());
		}







		///////////Updating////////////////

		GameManager::Update(deltaTime, window, event);

		GameManager::RemoveMarkedObjectsHelper();



		//Display whats actually been rendered
		window.display();


	}

	//Memory Cleanup At Terimination
	ResourceManager::ClearTextureMap();
	GameManager::ClearAllLists();


	return 0;
}
