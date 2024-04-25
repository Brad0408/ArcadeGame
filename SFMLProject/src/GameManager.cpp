#include "GameManager.h"

//Definition of the static member variable
std::vector<GameObject*> GameManager::GameObjectsVector;

void GameManager::AddGameObject(GameObject* gameObject)
{
	GetGameObjectVector().push_back(gameObject);
}



std::vector<GameObject*>& GameManager::GetGameObjectVector()
{
	return GameObjectsVector;
}





void GameManager::GetGameObjectNames(std::vector<GameObject*> GameObjectsVector)
{
	//Loop to just output the names of all created gameObject
	for (GameObject* gameobject : GameObjectsVector)
	{
		std::cout << "Stored GameObjects on the vector : " << gameobject->GetName() << std::endl;
	}
}

void GameManager::ClearGameObjectVector()
{
	GameObjectsVector.clear();
}
