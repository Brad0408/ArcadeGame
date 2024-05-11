#include "GameManager.h"
#include "Bullet.h"
#include "Enemy.h"

//Definition of the static member variable
std::list<std::unique_ptr<Bullet>> GameManager::BulletObjectsList;
std::list<std::unique_ptr<GameObject>> GameManager::GameObjectsList;
std::list<std::unique_ptr<Enemy>> GameManager::EnemyObjectsList;

std::unique_ptr<GameObject> GameManager::player = nullptr;
std::array<GameObject*, 4> GameManager::walls;


#pragma region AddingToLists

//Add to gameobject list - for raw pointers to the walls
void GameManager::AddGameObjectList(GameObject* gameObject)
{
	// Create a std::unique_ptr<GameObject> from the raw pointer
	std::unique_ptr<GameObject> gameObjectPtr(gameObject);

	// Store the unique pointer in the list
	GameObjectsList.push_back(std::move(gameObjectPtr));
}

//Add to the gameobject list 
void GameManager::AddGameObjectList(std::unique_ptr<GameObject> &gameObject)
{
	GameObjectsList.push_back(std::move(gameObject));
}


//Add to the game object list from the enemies list
void GameManager::AddGameObjectList(std::list<std::unique_ptr<Enemy>>& enemyList)
{
	for (auto& enemyPtr : enemyList)
	{
		// Move the unique pointer's ownership to a shared pointer
		std::unique_ptr<GameObject> gameObjectPtr = std::move(enemyPtr);

		AddGameObjectList(gameObjectPtr);
	}

	// Clear the original list after moving its elements
	enemyList.clear();
}



//Add to bullet list
void GameManager::AddBulletObjectList(std::unique_ptr<Bullet> bullet)
{
	// Store the unique pointer in the list
	BulletObjectsList.push_back(std::move(bullet));
}


void GameManager::AddEnemyObjectsList(std::unique_ptr<Enemy> enemy)
{
	EnemyObjectsList.push_back(std::move(enemy));
}


#pragma endregion


#pragma region GettingLists

std::list<std::unique_ptr<GameObject>> &GameManager::GetGameObjectList()
{
	return GameObjectsList;
}

std::list<std::unique_ptr<Bullet>> &GameManager::GetBulletsList()
{
	return BulletObjectsList;
}

std::list<std::unique_ptr<Enemy>> &GameManager::GetEnemyList()
{
	return EnemyObjectsList;
}
#pragma endregion


#pragma region GettingListNames

void GameManager::GetGameObjectListsNames()
{
	// Map to store the count of objects with each name
	std::unordered_map<std::string, int> nameCount;

	for (const auto& gameObjectPtr : GetGameObjectList())
	{
		std::string name = gameObjectPtr->GetName();
		std::string tag = gameObjectPtr->GetTag();

		// Check if the name already exists in the map
		if (nameCount.find(name) != nameCount.end())
		{
			// Increment the count for this name
			nameCount[name]++;
		}
		else
		{
			// Add the name to the map with count 1
			nameCount[name] = 1;
		}

		// Construct the name with count suffix if necessary
		std::string displayName = name;
		if (nameCount[name] > 1)
		{
			displayName += " " + std::to_string(nameCount[name]);
		}

		std::cout << "Stored GameObject on the list: " << displayName << " (Tag: " << tag << ")" << std::endl;
	}
}



void GameManager::GetEnemyListNames()
{
	for (const auto& enemyObjectPtr : GetEnemyList())
	{
		std::cout << "Stored EnemyObjects on the list : " << enemyObjectPtr->GetName() << std::endl;
	}

}

#pragma endregion


#pragma region Collisions

void GameManager::GenericCollision()
{
	// Declare objectA outside the inner loop
	GameObject* objectA = nullptr;

	// Check if player creation is in progress
	bool isResetEntities = false;

	// Check for collisions between game objects
	for (auto it = GetGameObjectList().begin(); it != GetGameObjectList().end(); ++it)
	{
		// Set objectA to the player object if it has not been set yet
		if ((*it)->GetTag() == "Player" && !objectA)
		{
			objectA = it->get();
		}

		for (auto jt = std::next(it); jt != GetGameObjectList().end(); ++jt)
		{
			GameObject* objectB = jt->get(); // Dereference the shared pointer to get the GameObject pointer

			// Skip collision checks if both objects are walls
			if (objectA && objectB && objectA->GetIsWall() && objectB->GetIsWall())
			{
				continue;
			}

			// Continue with your collision detection logic here
			if (objectA && objectB)
			{
				// Check for specific conditions before collision detection
				if ((objectA->GetIsEnemy() && objectB->GetIsEnemy()) || (objectA->GetIsPlayer() && objectB->GetIsPlayer()))
				{
					continue; // Skip collision detection between enemies or between players
				}

				BoxCollider* colliderA = objectA->GetComponent<BoxCollider>();
				BoxCollider* colliderB = objectB->GetComponent<BoxCollider>();

				// Ensure both colliders are valid before checking collision
				if (colliderA && colliderB && colliderA->CheckCollision(objectA, objectB))
				{
					//std::cout << "COLLISION" << std::endl;

					// Wall Detection
					if (objectA->GetIsWall() || objectB->GetIsWall())
					{
						//std::cout << "Wall Collision detected between objects " << objectA->GetName() << " and " << objectB->GetName() << std::endl;

						// Handle wall collision
						for (GameObject* wall : GetWalls())
						{
							BoxCollider::WallCollision(objectA, objectB);
						}
					}
					else
					{
						// Handle collision between non-wall objects
						if (!isResetEntities)
						{
							ClearEnemiesAndResetPlayer();
							isResetEntities = true;
						}

						//std::cout << "Collision detected between objects " << objectA->GetName() << " and " << objectB->GetName() << std::endl;

						//GameManager::GetGameObjectListsNames();
					}
				}
			}
		}
	}
}

void GameManager::BulletCollisions()
{
	// Check for collisions between bullets and other objects
	for (const auto& bullet : GetBulletsList())
	{
		// Check collision between bullet and other game objects
		for (const auto& gameObject : GetGameObjectList())
		{

			// Skip collision checks if the bullet belongs to the player
			if (gameObject->GetIsPlayer())
			{
				continue;
			}

			CircleCollider* bulletCollider = bullet->GetComponent<CircleCollider>();
			BoxCollider* gameObjectCollider = gameObject->GetComponent<BoxCollider>();

			// Ensure both colliders are valid before checking collision
			if (bulletCollider && gameObjectCollider && bulletCollider->BulletCollision(bullet.get(), gameObject.get()))
			{
				// Mark bullet and object for removal
				if (gameObject->GetIsWall())
				{
					bullet->MarkForRemoval();
				}
				else
				{
					gameObject->MarkForRemoval();
					bullet->MarkForRemoval();

					//std::cout << "Collision detected between bullet " << bullet << " and " << gameObject->GetName() << std::endl;
				}
			}
		}
	}
}

#pragma endregion


#pragma region ClearingLists
void GameManager::ClearAllLists()
{
	BulletObjectsList.clear();
	GameObjectsList.clear();
	EnemyObjectsList.clear();
}

void GameManager::ClearGameObjectList()
{
	for (auto& gameObject : GetGameObjectList())
	{
		gameObject->MarkForRemoval();
	}
}

void GameManager::ClearEnemiesAndResetPlayer()
{

	for (auto& gameObject : GetGameObjectList())
	{
		// Check if the current object has the tag "Enemy"
		if (gameObject->GetTag() == "Enemy")
		{
			gameObject->MarkForRemoval();
		}
		else if (gameObject->GetTag() == "Player")
		{
			gameObject->SetLocation(500, 450);
		}
	}
}
#pragma endregion 



void GameManager::RemoveMarkedObjectsHelper()
{
	RemoveMarkedObjectsList<Bullet>(GetBulletsList());
	RemoveMarkedObjectsList<GameObject>(GetGameObjectList());
}




void GameManager::Update(float deltaTime, sf::RenderWindow& window, sf::Event& event)
{
	for (auto& bulletPtr : GetBulletsList())
	{

		if (bulletPtr)
		{
			bulletPtr->Update(deltaTime);
			bulletPtr->GetComponent<CircleCollider>()->DrawOutlines(bulletPtr->GetCircleShape());
		}
	}

	for (auto& gameObject : GetGameObjectList())
	{
		// Check if the current object has the tag "Player"
		if (gameObject->GetTag() == "Player")
		{
			PlayerComponent* playerComponent = gameObject->GetComponent<PlayerComponent>();
			if (playerComponent)
			{
				playerComponent->Update(deltaTime);
				playerComponent->CalculateFiringPointRotation(window);

				// Check for left mouse button press event
				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
				{
					gameObject->SetIsShooting(true);
				}

				// Check for left mouse button release event
				if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
				{
					gameObject->SetIsShooting(false);
				}

				if (GetPlayer()->GetIsShooting())
				{
					playerComponent->Shooting();
				}

			}

		}
		else if (gameObject->GetTag() == "Enemy")
		{
			auto enemy = dynamic_cast<Enemy*>(gameObject.get());
			if (enemy)
			{
				enemy->Update(deltaTime, GetPlayer()->GetLocation());
			}
		}
	}


}

#pragma region SpawningEnemies

std::vector<AG::Vector2<float>> GameManager::GenerateRandomSpawnLocations(int numSpawnLocations)
{
	std::vector<AG::Vector2<float>> spawnLocations;
	AG::Vector2<float> playerLocation = GetPlayer()->GetLocation();
	float minDistance = 200.0f;

	// Initialize random number generator
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> disX(80.0f, 850.0f); // Range for X coordinate
	std::uniform_real_distribution<float> disY(80.0f, 850.0f); // Range for Y coordinate

	// Generate random spawn locations
	for (int i = 0; i < numSpawnLocations; ++i)
	{
		float x, y;
		float distance;
		do
		{
			// Generate random coordinates
			x = disX(gen);
			y = disY(gen);

			// Calculate distance between player spawn location and current spawn location
			distance = std::sqrt((x - playerLocation.x) * (x - playerLocation.x) + (y - playerLocation.y) * (y - playerLocation.y));

			// Check if the distance is greater than the minimum distance
		} while (distance < minDistance);

		spawnLocations.push_back(AG::Vector2<float>(x, y));
	}

	return spawnLocations;
}

void GameManager::CreateEnemyPool(int numEnemies)
{
	std::cout << "ENEMEIS CREATED" << std::endl;

	//GetPlayer()->SetLocation(500, 450);
	//GetPlayer()->GetRectangleShape().setPosition(GetPlayer()->GetLocation());

	EnemyObjectsList.clear();

	std::vector<AG::Vector2<float>> spawnLocations = GenerateRandomSpawnLocations(numEnemies);

	for (const auto& spawnLocation : spawnLocations)
	{
		// Create a shared pointer to a dynamically allocated Enemy object
		std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>(spawnLocation);

		AddEnemyObjectsList(std::move(newEnemy));

	}
}

#pragma endregion

#pragma region Player
void GameManager::CreatePlayer()
{
	std::cout << "PLAYER CREATED" << std::endl;

	// Check if a player object already exists
	if (!player)
	{
		// Create a unique pointer to a new player object
		player = std::make_unique<GameObject>();

		player->AddComponent<PlayerComponent>();


		// Add the player object to the GameObjectsList
		AddGameObjectList(player);
	}
	else
	{
		std::cout << "A player object already exists." << std::endl;
	}
}


std::unique_ptr<GameObject> &GameManager::GetPlayer()
{
	for (auto& gameObject : GetGameObjectList())
	{
		// Check if the current object has the tag "Player"
		if (gameObject->GetTag() == "Player")
		{
			// Return a reference to the player object
			return gameObject;
		}
	}
}

#pragma endregion

#pragma region Walls

void GameManager::CreateWalls()
{

	std::array<std::string, 4> wallNames = { "TopWall", "LeftWall", "BottomWall", "RightWall" };

	for (int i = 0; i < 4; ++i)
	{
		walls[i] = new GameObject();
		AddGameObjectList(walls[i]);
		walls[i]->SetName(wallNames[i]);
		walls[i]->SetTag("Wall");
		walls[i]->AddComponent<BoxCollider>();
	}

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
}

std::array<GameObject*, 4>& GameManager::GetWalls()
{
	return walls;
}

#pragma endregion
