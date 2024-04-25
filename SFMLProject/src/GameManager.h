#pragma once
#include <GameObject.h>


class GameManager : public Object
{

public:
	static void AddGameObject(GameObject* gameObject);

	static std::vector<GameObject*> &GetGameObjectVector();

	static void GetGameObjectNames(std::vector<GameObject*> GameObjectsVector);

	static void ClearGameObjectVector();

private:
	//Vector that stores all the created gameObjects
	static std::vector<GameObject*> GameObjectsVector;

};
