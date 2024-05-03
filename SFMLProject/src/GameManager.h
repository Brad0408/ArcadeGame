#pragma once
#include <GameObject.h>

class Bullet;


class GameManager : public Object
{

public:
	static void AddGameObject(GameObject* gameObject);

	static void AddBulletObject(Bullet* bullet);
	static void RemoveBullet(Bullet* bullet);

	static std::vector<GameObject*> &GetGameObjectVector();
	static std::vector<Bullet*> &GetBulletsVector();

	static void GetGameObjectNames(std::vector<GameObject*> GameObjectsVector);

	static void ClearAllVectors();
	static void ClearGameObjectVector();
	static void ClearBulletVector();

	static void RemoveMarkedGameObjects();
	static void RemoveMarkedBullets();

private:
	//Vector that stores all the created gameObjects
	static std::vector<GameObject*> GameObjectsVector;
	static std::vector<Bullet*> BulletsVector;

};
