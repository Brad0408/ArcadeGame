#include "GameManager.h"
#include "Bullet.h"
#include "Enemy.h"

//Definition of the static member variable
std::list<std::shared_ptr<Bullet*>> GameManager::BulletObjectsList;
std::list<std::shared_ptr<GameObject*>> GameManager::GameObjectsList;
std::list<std::shared_ptr<Enemy*>> GameManager::EnemyObjectsList;

std::vector<GameObject*> GameManager::GameObjectsVector;
std::vector<Bullet*> GameManager::BulletsVector;

//Add to gameobject vector
void GameManager::AddGameObject(GameObject* gameObject)
{
	GetGameObjectVector().push_back(gameObject);
}

//Add to gameobject list
void GameManager::AddGameObjectList(GameObject* gameObject)
{
	std::shared_ptr<GameObject*> gameObjectPtr(new GameObject * (gameObject));
	GameObjectsList.push_back(gameObjectPtr);
}

//Add to bullet vector
void GameManager::AddBulletObject(Bullet* bullet)
{
	GetBulletsVector().push_back(bullet);
}

//Add to bullet list
void GameManager::AddBulletObjectList(Bullet* bullet)
{
	std::shared_ptr<Bullet*> bulletObjectPtr(new Bullet * (bullet));
	BulletObjectsList.push_back(bulletObjectPtr);
}

void GameManager::AddEnemyObjectsList(Enemy* enemy)
{
	std::shared_ptr<Enemy*> enemyObjectPtr(new Enemy * (enemy));
	EnemyObjectsList.push_back(enemyObjectPtr);
}


std::vector<GameObject*>& GameManager::GetGameObjectVector()
{
	return GameObjectsVector;
}

std::vector<Bullet*>& GameManager::GetBulletsVector()
{
	return BulletsVector;
}

std::list<std::shared_ptr<GameObject*>> &GameManager::GetGameObjectList()
{
	return GameObjectsList;
}

std::list<std::shared_ptr<Bullet*>> &GameManager::GetBulletsList()
{
	return BulletObjectsList;
}

std::list<std::shared_ptr<Enemy*>>& GameManager::GetEnemyList()
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

void GameManager::GetGameObjectListsNames(std::list<std::shared_ptr<GameObject*>>& GameObjectsList)
{
	for (const auto& gameObjectPtr : GameObjectsList)
	{
		GameObject* gameObject = *gameObjectPtr; // Dereference the shared pointer to get the GameObject pointer
		std::cout << "Stored GameObjects on the list : " << gameObject->GetName() << std::endl;
	}

}

void GameManager::ClearAllVectors()
{
	GameObjectsVector.clear();
	BulletsVector.clear();
}

void GameManager::ClearAllLists()
{
	BulletObjectsList.clear();
	GameObjectsList.clear();
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

void GameManager::Update(float deltaTime)
{
	for (auto& bulletPtr : GetBulletsList())
	{
		if (bulletPtr)
		{
			Bullet* bullet = *bulletPtr;
			bullet->Update(deltaTime);
		}
	}
}

std::vector<AG::Vector2<float>> GameManager::GenerateRandomSpawnLocations(int numSpawnLocations)
{
	std::vector<AG::Vector2<float>> spawnLocations;


	// Initialize random number generator
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> disX(0.0f, 800.0f); // Range for X coordinate
	std::uniform_real_distribution<float> disY(0.0f, 800.0f); // Range for Y coordinate

	// Generate random spawn locations
	for (int i = 0; i < numSpawnLocations; ++i)
	{
		float x = disX(gen);
		float y = disY(gen);
		spawnLocations.push_back(AG::Vector2<float>(x, y));
	}

	return spawnLocations;
}

void GameManager::CreateEnemyPool(int numEnemies)
{
	EnemyObjectsList.clear();

	std::vector<AG::Vector2<float>> spawnLocations = GenerateRandomSpawnLocations(numEnemies);

	for (const auto& spawnLocation : spawnLocations)
	{
		// Create a new enemy object using the spawn location directly
		Enemy* newEnemy = new Enemy(spawnLocation);

		AddEnemyObjectsList(newEnemy);
	}

	
}
