#pragma once
#include <gameObject_concept.h>

class Bullet;
class Enemy;


class GameManager : public Object
{

public:
	static void AddGameObjectList(std::unique_ptr<GameObject> &gameObject);
	static void AddGameObjectList(GameObject* gameObject);
	static void AddGameObjectList(std::list<std::unique_ptr<Enemy>>& enemyList);
	static void AddBulletObjectList(std::unique_ptr<Bullet> bullet);
	static void AddEnemyObjectsList(std::unique_ptr<Enemy> enemy);


	static std::list<std::unique_ptr<GameObject>> &GetGameObjectList();
	static std::list<std::unique_ptr<Bullet>> &GetBulletsList();
	static std::list<std::unique_ptr<Enemy>> &GetEnemyList();
	static std::unique_ptr<GameObject>& GetPlayer();
	static std::array<GameObject*, 4>& GetWalls();


	static void GetGameObjectListsNames();
	static void GetEnemyListNames();


	static void GenericCollision();
	static void BulletCollisions();


	static void ClearAllLists();
	static void ClearGameObjectList();
	static void ClearEnemiesAndResetPlayer();



	static void RemoveMarkedObjectsHelper();

	static void Update(float deltaTime, sf::RenderWindow& window, sf::Event& event);


	static std::vector<AG::Vector2<float>> GenerateRandomSpawnLocations(int numSpawnLocations);
	static void CreateEnemyPool(int numEnemies);
	static void CreatePlayer();
	static void CreateWalls();



private:

	//Alternative lists instead of vectors and pointer types, helped with some memory issues
	static std::list<std::unique_ptr<Bullet>> BulletObjectsList;
	static std::list<std::unique_ptr<GameObject>> GameObjectsList;
	static std::list<std::unique_ptr<Enemy>> EnemyObjectsList;


	static std::unique_ptr<GameObject> player;
	static std::array<GameObject*, 4> walls;



public:
	template <class T> requires isGameObject<T> static void RemoveMarkedObjectsList(std::list<std::unique_ptr<T>>& objects)
	{
		objects.erase(std::remove_if(objects.begin(), objects.end(), [](const std::unique_ptr<T>& objectPtr)
		{
			return (objectPtr && objectPtr->ShouldRemove());
		}), objects.end());
	}


	//template <class T> requires isGameObject<T> static void RemoveMarkedObjectsList(std::list<std::shared_ptr<T>>& objects)
	//{
	//	auto it = std::remove_if(objects.begin(), objects.end(), [](std::shared_ptr<T>& objectPtr)
	//		{
	//			T* object = *objectPtr; // Dereference the shared pointer to get the pointer to T
	//			if (object && object->ShouldRemove())
	//			{
	//				delete object;
	//				return true;
	//			}
	//			return false;
	//		});
	//}

	template <class T> requires isGameObject<T> static void RemoveMarkedObjectsVector(std::vector<T*>& objects)
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
