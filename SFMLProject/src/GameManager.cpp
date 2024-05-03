#include "GameManager.h"
#include "Bullet.h"

//Definition of the static member variable
std::vector<GameObject*> GameManager::GameObjectsVector;
std::vector<Bullet*> GameManager::BulletsVector;

void GameManager::AddGameObject(GameObject* gameObject)
{
	GetGameObjectVector().push_back(gameObject);
}

void GameManager::AddBulletObject(Bullet* bullet)
{
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
}

void GameManager::ClearGameObjectVector()
{
	GameObjectsVector.clear();
}

void GameManager::ClearBulletVector()
{
	BulletsVector.clear();
}





void GameManager::RemoveBullet(Bullet* bullet)
{
	std::vector<Bullet*>& bullets = GetBulletsVector();
	auto it = std::find(bullets.begin(), bullets.end(), bullet);
	if (it != bullets.end())
	{
		bullets.erase(it);
		delete bullet;
	}
}




void GameManager::RemoveMarkedGameObjects()
{
	auto it = std::remove_if(GetGameObjectVector().begin(), GetGameObjectVector().end(), [](GameObject* gameObject)
	{
		bool shouldReturn = gameObject->ShouldRemoveGameObject();

		if (shouldReturn)
		{
			delete gameObject;
		}

		return shouldReturn;

	});
	GetGameObjectVector().erase(it, GetGameObjectVector().end());
}

void GameManager::RemoveMarkedBullets()
{
	auto it = std::remove_if(GetBulletsVector().begin(), GetBulletsVector().end(), [](Bullet* bullet)
	{
		bool shouldReturn = bullet->ShouldRemove();

		if (shouldReturn)
		{
			delete bullet;
		}

		return shouldReturn;
		
	});
	GetBulletsVector().erase(it, GetBulletsVector().end());
}
