#pragma once
#include <gameObject_concept.h>

class Bullet;


class GameManager : public Object
{

public:
	static void AddGameObject(GameObject* gameObject);
	static void AddGameObjectList(GameObject* gameObject);

	static void AddBulletObject(Bullet* bullet);
	static void AddBulletObjectList(Bullet* bullet);

	static void RemoveBullet(Bullet* bullet);

	static std::vector<GameObject*>& GetGameObjectVector();
	static std::vector<Bullet*>& GetBulletsVector();

	static std::list<std::shared_ptr<GameObject*>> &GetGameObjectList();
	static std::list<std::shared_ptr<Bullet*>> &GetBulletsList();

	static void GetGameObjectNames(std::vector<GameObject*> GameObjectsVector);
	static void GetGameObjectListsNames(std::list<std::shared_ptr<GameObject*>>& GameObjectsList);

	static void ClearAllVectors();
	static void ClearAllLists();


	static void ClearGameObjectVector();
	static void ClearGameObjectList();

	static void ClearBulletVector();
	static void ClearBulletList();

	static void RemoveMarkedGameObjects();
	static void RemoveMarkedBullets();
	static void RemoveMarkedObjectsHelper();

	static void Update(float deltaTime);

private:
	//Vector that stores all the created gameObjects
	static std::vector<GameObject*> GameObjectsVector;
	static std::vector<Bullet*> BulletsVector;

	static std::list<std::shared_ptr<Bullet*>> BulletObjectsList;
	static std::list<std::shared_ptr<GameObject*>> GameObjectsList;


public:
	template <class T> requires isGameObject<T> static void RemoveMarkedObjectsList(std::list<std::shared_ptr<T*>>& objects)
	{
		auto it = std::remove_if(objects.begin(), objects.end(), [](std::shared_ptr<T*>& objectPtr)
			{
				T* object = *objectPtr; // Dereference the shared pointer to get the pointer to T
				if (object && object->ShouldRemove())
				{
					return true;
				}
				return false;
			});

		objects.erase(it, objects.end());


	}


	template <class T> requires isGameObject<T> static void RemoveMarkedObjects(std::vector<T*>& objects)
	{
		auto it = std::remove_if(objects.begin(), objects.end(), [](T* object)
			{
				if (object && object->ShouldRemove())
				{
					delete object;
					return true;
				}
				return false;
			});

		objects.erase(it, objects.end());
	}
};
