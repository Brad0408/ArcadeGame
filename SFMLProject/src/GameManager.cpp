#include "GameManager.h"
#include "Bullet.h"
#include "Enemy.h"

//Definition of the static member variable
std::list<std::unique_ptr<Bullet>> GameManager::BulletObjectsList;
std::list<std::unique_ptr<GameObject>> GameManager::GameObjectsList;

std::list<std::unique_ptr<Enemy>> GameManager::EnemyObjectsList;

std::vector<GameObject*> GameManager::GameObjectsVector;
std::vector<Bullet*> GameManager::BulletsVector;
std::vector<Enemy*> GameManager::EnemyVector;

std::unique_ptr<GameObject> GameManager::player = nullptr;


//Add to gameobject vector
void GameManager::AddGameObject(GameObject* gameObject)
{
	GetGameObjectVector().push_back(gameObject);
}



//Add to gameobject list
void GameManager::AddGameObjectList(GameObject* gameObject)
{
	// Create a std::unique_ptr<GameObject> from the raw pointer
	std::unique_ptr<GameObject> gameObjectPtr(gameObject);

	// Store the unique pointer in the list
	GameObjectsList.push_back(std::move(gameObjectPtr));
}


void GameManager::AddGameObject(std::unique_ptr<GameObject> gameObject)
{
	GameObjectsList.push_back(std::move(gameObject));
}


void GameManager::AddGameObjectList(std::list<std::unique_ptr<Enemy>>& enemyList)
{
	for (auto& enemyPtr : enemyList)
	{
		// Move the unique pointer's ownership to a shared pointer
		std::unique_ptr<GameObject> gameObjectPtr = std::move(enemyPtr);

		// Add the shared pointer to the GameObject list
		GameObjectsList.push_back(std::move(gameObjectPtr)); // Corrected this line
	}

	// Clear the original list after moving its elements
	enemyList.clear();
}

//Add to bullet vector
void GameManager::AddBulletObject(Bullet* bullet)
{
	GetBulletsVector().push_back(bullet);
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


std::vector<GameObject*>& GameManager::GetGameObjectVector()
{
	return GameObjectsVector;
}

std::vector<Bullet*>& GameManager::GetBulletsVector()
{
	return BulletsVector;
}

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

void GameManager::GetGameObjectNames(std::vector<GameObject*> GameObjectsVector)
{
	//Loop to just output the names of all created gameObject
	for (GameObject* gameobject : GameObjectsVector)
	{
		std::cout << "Stored GameObjects on the vector : " << gameobject->GetName() << std::endl;
	}
}

void GameManager::GetGameObjectListsNames(std::list<std::unique_ptr<GameObject>>& GameObjectsList)
{
	for (const auto& gameObjectPtr : GameObjectsList)
	{
		std::cout << "Stored GameObjects on the list : " << gameObjectPtr->GetName() << std::endl;
	}

}

void GameManager::GetEnemyListNames(std::list<std::unique_ptr<Enemy>>& EnemyObjectsList)
{
	for (const auto& enemyObjectPtr : EnemyObjectsList)
	{
		std::cout << "Stored EnemyObjects on the list : " << enemyObjectPtr->GetName() << std::endl;
	}

}

void GameManager::ClearAllVectors()
{
	GameObjectsVector.clear();
	BulletsVector.clear();
	EnemyVector.clear();
}

void GameManager::ClearAllLists()
{
	BulletObjectsList.clear();
	GameObjectsList.clear();
	EnemyObjectsList.clear();
}

void GameManager::ClearGameObjectVector()
{
	GameObjectsVector.clear();
}

void GameManager::ClearGameObjectList()
{
	GameObjectsList.clear();
}

void GameManager::ClearBulletVector()
{
	BulletsVector.clear();
}

void GameManager::ClearBulletList()
{
	BulletObjectsList.clear();
}





void GameManager::RemoveBullet(Bullet* bullet)
{
	std::vector<Bullet*>& bullets = GetBulletsVector();

	//auto& bullets = GetBulletsList();
	auto it = std::find(bullets.begin(), bullets.end(), bullet);
	if (it != bullets.end())
	{
		bullets.erase(it);
		delete bullet;
	}
}





void GameManager::RemoveMarkedGameObjects()
{
	//auto &gameObjects = GetGameObjectList();
	auto &gameObjects = GetGameObjectVector();

	auto it = std::remove_if(gameObjects.begin(), gameObjects.end(), [](GameObject* gameObject)
	{
		bool shouldReturn = gameObject->ShouldRemove();

		if (shouldReturn)
		{
			delete gameObject;
			return true;
		}

		return false;
	});

	gameObjects.erase(it, gameObjects.end());
}

void GameManager::RemoveMarkedBullets()
{
	//auto &bullets = GetBulletsList();
	auto &bullets = GetBulletsVector();

	auto it = std::remove_if(bullets.begin(), bullets.end(), [](Bullet* bullet)
	{
		bool shouldReturn = bullet->ShouldRemove();

		if (shouldReturn)
		{
			delete bullet;
			return true;
		}

		return false;
	});

	bullets.erase(it, bullets.end());
}

void GameManager::RemoveMarkedObjectsHelper()
{
	GameManager::RemoveMarkedObjectsList<Bullet>(GetBulletsList());
	GameManager::RemoveMarkedObjectsList<GameObject>(GetGameObjectList());

	//GameManager::RemoveMarkedObjects(GetGameObjectVector());
	//GameManager::RemoveMarkedObjects(GetBulletsVector());

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
			auto playerComponent = gameObject->GetComponent<PlayerComponent>();
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

				if (GameManager::GetPlayer().GetIsShooting())
				{
					playerComponent->Shooting();
				}

			}

		}
	}
}

std::vector<AG::Vector2<float>> GameManager::GenerateRandomSpawnLocations(int numSpawnLocations)
{
	std::vector<AG::Vector2<float>> spawnLocations;
	AG::Vector2<float> playerSpawnLocation = AG::Vector2<float>(500, 450);
	float minDistance = 200.0f;

	// Initialize random number generator
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> disX(100.0f, 850.0f); // Range for X coordinate
	std::uniform_real_distribution<float> disY(100.0f, 850.0f); // Range for Y coordinate

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
			distance = std::sqrt((x - playerSpawnLocation.x) * (x - playerSpawnLocation.x) + (y - playerSpawnLocation.y) * (y - playerSpawnLocation.y));

			// Check if the distance is greater than the minimum distance
		} while (distance < minDistance);

		spawnLocations.push_back(AG::Vector2<float>(x, y));
	}

	return spawnLocations;
}

void GameManager::CreateEnemyPool(int numEnemies)
{
	std::cout << "ENEMEIS CREATED" << std::endl;

	EnemyObjectsList.clear();

	std::vector<AG::Vector2<float>> spawnLocations = GenerateRandomSpawnLocations(numEnemies);

	for (const auto& spawnLocation : spawnLocations)
	{
		// Create a shared pointer to a dynamically allocated Enemy object
		std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>(spawnLocation);

		// Pass the shared pointer to GameManager::AddEnemyObjectList
		GameManager::AddEnemyObjectsList(std::move(newEnemy));

	}

	
}

void GameManager::CreatePlayer()
{
	// Create a unique pointer to a new player object
	player = std::make_unique<GameObject>();

	player->AddComponent<PlayerComponent>();
	player->AddComponent<BoxCollider>();
	player->GetComponent<BoxCollider>()->DrawOutlines(player->GetRectangleShape());


	// Add the player object to the GameObjectsList
	GameObjectsList.push_back(std::move(player));

}

GameObject& GameManager::GetPlayer()
{
	// Loop through the GameObjectsList to find the player object
	for (auto& gameObject : GetGameObjectList())
	{
		// Check if the current object has the tag "Player"
		if (gameObject->GetTag() == "Player")
		{
			// Return a reference to the player object
			return *gameObject;
		}
	}

	// If no player object is found, throw an exception
	std::cout << "Player not found" << std::endl;
	throw std::runtime_error("Player not found");
}
