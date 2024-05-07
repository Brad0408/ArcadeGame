#include "GameManager.h"
#include "Bullet.h"

//Definition of the static member variable
//std::list<std::shared_ptr<Bullet*>> GameManager::BulletObjectsList;
//std::list<std::shared_ptr<GameObject*>> GameManager::GameObjectsList;

std::vector<GameObject*> GameManager::GameObjectsVector;
std::vector<Bullet*> GameManager::BulletsVector;

void GameManager::AddGameObject(GameObject* gameObject)
{
	//std::shared_ptr<GameObject*> gameObjectPtr = std::make_shared<GameObject*>(gameObjectPtr);
	//GameObjectsList.push_back(gameObjectPtr);

	GetGameObjectVector().push_back(gameObject);

}

void GameManager::AddBulletObject(Bullet* bullet)
{
	//std::shared_ptr<Bullet*> bulletObjectPtr = std::make_shared<Bullet*>(bulletObjectPtr);
	//BulletObjectsList.push_back(bulletObjectPtr);


	GetBulletsVector().push_back(bullet);
}


std::vector<GameObject*>& GameManager::GetGameObjectVector()
{
	return GameObjectsVector;
}

std::vector<Bullet*>& GameManager::GetBulletsVector()
{
	return BulletsVector;
}

//std::list<std::shared_ptr<GameObject*>> &GameManager::GetGameObjectList()
//{
//	return GameObjectsList;
//}
//
//std::list<std::shared_ptr<Bullet*>> &GameManager::GetBulletsList()
//{
//	return BulletObjectsList;
//}

void GameManager::GetGameObjectNames(std::vector<GameObject*> GameObjectsVector)
{
	//Loop to just output the names of all created gameObject
	for (GameObject* gameobject : GameObjectsVector)
	{
		std::cout << "Stored GameObjects on the vector : " << gameobject->GetName() << std::endl;
	}
}

void GameManager::ClearAllVectors()
{
	GameObjectsVector.clear();
	BulletsVector.clear();

	//BulletObjectsList.clear();
	//GameObjectsList.clear();
}

void GameManager::ClearGameObjectVector()
{
	GameObjectsVector.clear();

	//GameObjectsList.clear();
}

void GameManager::ClearBulletVector()
{
	BulletsVector.clear();

	//BulletObjectsList.clear();
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
	//GameManager::RemoveMarkedObjectsList<Bullet*>(GetBulletsList());
	//GameManager::RemoveMarkedObjects<GameObject>(GetGameObjectList());


	GameManager::RemoveMarkedObjects(GetGameObjectVector());
	GameManager::RemoveMarkedObjects(GetBulletsVector());

}

void GameManager::Update(float deltaTime)
{
	for (Bullet* bullet : GetBulletsVector())
	{
		if (bullet)
		{	
			bullet->Update(deltaTime);
		}
	}



	// Remove bullets marked for removal
	//RemoveMarkedObjectsList(GetBulletsList());
}
