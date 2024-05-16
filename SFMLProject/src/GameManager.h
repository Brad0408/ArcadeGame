#pragma once
#include <gameObject_concept.h>

class Bullet;
class Enemy;
class Family;


class GameManager : public Object
{

public:
	static void AddGameObjectList(std::unique_ptr<GameObject> &gameObject);
	static void AddGameObjectList(std::unique_ptr<Family> &gameObject);
	static void AddGameObjectList(GameObject* gameObject);
	static void AddGameObjectList(std::list<std::unique_ptr<Enemy>>& enemyList);

	static void AddBulletObjectList(std::unique_ptr<Bullet> bullet);
	static void AddEnemyObjectsList(std::unique_ptr<Enemy> enemy);


	static std::list<std::unique_ptr<GameObject>>& GetGameObjectList() { return GameObjectsList; }
	static std::list<std::unique_ptr<Bullet>>& GetBulletsList() { return BulletObjectsList; }
	static std::list<std::unique_ptr<Enemy>>& GetEnemyList() { return EnemyObjectsList; }
	static std::array<GameObject*, 4>& GetWalls() { return walls; }
	static std::unique_ptr<GameObject>& GetPlayer();
	static int GetEnemyCount();


	static void GetGameObjectListsNames();
	static void GetEnemyListNames();


	static void GenericCollision();
	static void BulletCollisions();
	static void FamilyCollisions();


	static void ClearAllLists();
	static void ClearGameObjectList();
	static void ClearAndResetEntites();
	static void ClearAnyBullets();
	static void RemoveMarkedObjectsHelper();
	static void ClearFont() { font = sf::Font(); }



	static void Update(float deltaTime, sf::RenderWindow& window);


	static std::vector<AG::Vector2<float>> GenerateRandomSpawnLocations(int numSpawnLocations);
	static void CreateEnemyPool(int numEnemies);
	static float GenerateRandomEnemySpeeds();
	static void CreatePlayer();
	static void CreateWalls();
	static void CreateFamily(int numFamilies);

	static void SettingFont();
	static void SettingGameplayText();
	static void UpdateScore(int points);
	static void UpdateLives(int life, bool increaseLives);
	static void UpdateWaveCounter(int addCount);
	static void IncreaseWaveCounter() { waves++;  wavesText.setString("Wave: " + std::to_string(waves)); waveKills = 0; }	//Debug
	static sf::Text& GetScoreTexts() { return scoreText; }
	static sf::Text& GetLivesTexts() { return livesText; }
	static sf::Text& MainMenuText();
	static sf::Text& MainMenuRobotronText();
	static int GetWaveCount() { return waves; }

	static void TextRender(sf::RenderWindow& window);

	static void StartGame();
	static void RestartGame();
	static void GameOver();
	static bool &IsGameStarted() { return gameStarted; }
	static bool &IsGameOver() { return gameOver; }
	static void SetGameOverScreen();

private:
	//Alternative pointer types, helped with some memory issues
	static std::list<std::unique_ptr<Bullet>> BulletObjectsList;
	static std::list<std::unique_ptr<GameObject>> GameObjectsList;
	static std::list<std::unique_ptr<Enemy>> EnemyObjectsList;


	static std::unique_ptr<GameObject> player;
	static int waveKills;

	//Raw pointers for walls - wont ever be deleted
	static std::array<GameObject*, 4> walls;

	static sf::Font font;
	static sf::Text scoreText;
	static sf::Text livesText;
	static sf::Text wavesText;
	static sf::Text gameOverText;
	static sf::Text playthegameText;
	static sf::Text robotronText;
	static sf::Text hightScoreText;
	static sf::Text highWaveText;


	static int playerScore;
	static int playerLives;
	static int highScore;
	static int highWaveScore;
	static int waves;
	static bool gameOver;
	static bool gameStarted;

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
