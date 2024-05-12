#include "GameManager.h"
#include "Bullet.h"
#include "Enemy.h"

//Definition of the static member variable
std::list<std::unique_ptr<Bullet>> GameManager::BulletObjectsList;
std::list<std::unique_ptr<GameObject>> GameManager::GameObjectsList;
std::list<std::unique_ptr<Enemy>> GameManager::EnemyObjectsList;

std::unique_ptr<GameObject> GameManager::player = nullptr;
std::array<GameObject*, 4> GameManager::walls;

sf::Font GameManager::font;
sf::Text GameManager::scoreText;
sf::Text GameManager::livesText;
sf::Text GameManager::wavesText;

int GameManager::playerScore = 0;
int GameManager::playerLives = 5;
int GameManager::waveKills = 0;
int GameManager::waves = 0;

#pragma region AddingToLists

//Add to gameobject list - for raw pointers to the walls
void GameManager::AddGameObjectList(GameObject* gameObject)
{
	// Create a std::unique_ptr<GameObject> from the raw pointer
	std::unique_ptr<GameObject> gameObjectPtr(gameObject);

	// Store the unique pointer in the list
	GameObjectsList.push_back(std::move(gameObjectPtr));
}

//Add to the gameobject list 
void GameManager::AddGameObjectList(std::unique_ptr<GameObject> &gameObject)
{
	GameObjectsList.push_back(std::move(gameObject));
}


//Add to the game object list from the enemies list
void GameManager::AddGameObjectList(std::list<std::unique_ptr<Enemy>>& enemyList)
{
	for (auto& enemyPtr : enemyList)
	{
		// Move the unique pointer's ownership to a shared pointer
		std::unique_ptr<GameObject> gameObjectPtr = std::move(enemyPtr);

		AddGameObjectList(gameObjectPtr);
	}

	// Clear the original list after moving its elements
	enemyList.clear();
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


#pragma endregion




#pragma region GettingListNames

void GameManager::GetGameObjectListsNames()
{
	// Map to store the count of objects with each name
	std::unordered_map<std::string, int> nameCount;

	for (const auto& gameObjectPtr : GetGameObjectList())
	{
		std::string name = gameObjectPtr->GetName();
		std::string tag = gameObjectPtr->GetTag();

		// Check if the name already exists in the map
		if (nameCount.find(name) != nameCount.end())
		{
			// Increment the count for this name
			nameCount[name]++;
		}
		else
		{
			// Add the name to the map with count 1
			nameCount[name] = 1;
		}

		// Construct the name with count suffix if necessary
		std::string displayName = name;
		if (nameCount[name] > 1)
		{
			displayName += " " + std::to_string(nameCount[name]);
		}

		std::cout << "Stored GameObject on the list: " << displayName << " (Tag: " << tag << ")" << std::endl;
	}
}



void GameManager::GetEnemyListNames()
{
	for (const auto& enemyObjectPtr : GetEnemyList())
	{
		std::cout << "Stored EnemyObjects on the list : " << enemyObjectPtr->GetName() << std::endl;
	}
}

#pragma endregion


#pragma region Collisions

void GameManager::GenericCollision()
{
	// Declare objectA outside the inner loop
	GameObject* objectA = nullptr;

	// Check if player creation is in progress
	bool isResetEntities = false;

	// Check for collisions between game objects
	for (auto it = GetGameObjectList().begin(); it != GetGameObjectList().end(); ++it)
	{
		// Set objectA to the player object if it has not been set yet
		if ((*it)->GetTag() == "Player" && !objectA)
		{
			objectA = it->get();
		}

		for (auto jt = std::next(it); jt != GetGameObjectList().end(); ++jt)
		{
			GameObject* objectB = jt->get(); // Dereference the shared pointer to get the GameObject pointer

			// Skip collision checks if both objects are walls
			if (objectA && objectB && objectA->GetIsWall() && objectB->GetIsWall())
			{
				continue;
			}

			// Continue with your collision detection logic here
			if (objectA && objectB)
			{
				// Check for specific conditions before collision detection
				if ((objectA->GetIsEnemy() && objectB->GetIsEnemy()) || (objectA->GetIsPlayer() && objectB->GetIsPlayer()))
				{
					continue; // Skip collision detection between enemies or between players
				}

				BoxCollider* colliderA = objectA->GetComponent<BoxCollider>();
				BoxCollider* colliderB = objectB->GetComponent<BoxCollider>();

				// Ensure both colliders are valid before checking collision
				if (colliderA && colliderB && colliderA->CheckCollision(objectA, objectB))
				{
					//std::cout << "COLLISION" << std::endl;

					// Wall Detection
					if (objectA->GetIsWall() || objectB->GetIsWall())
					{
						//std::cout << "Wall Collision detected between objects " << objectA->GetName() << " and " << objectB->GetName() << std::endl;

						// Handle wall collision
						for (GameObject* wall : GetWalls())
						{
							BoxCollider::WallCollision(objectA, objectB);
						}
					}
					else
					{
						// Handle collision between non-wall objects
						if (!isResetEntities)
						{
							ClearEnemiesAndResetPlayer();
							UpdateLives(1, false);
							isResetEntities = true;
							waveKills = 0;
						}

						//std::cout << "Collision detected between objects " << objectA->GetName() << " and " << objectB->GetName() << std::endl;

						//GameManager::GetGameObjectListsNames();
					}
				}
			}
		}
	}
}

void GameManager::BulletCollisions()
{
	// Check for collisions between bullets and other objects
	for (const auto& bullet : GetBulletsList())
	{
		// Check collision between bullet and other game objects
		for (const auto& gameObject : GetGameObjectList())
		{

			// Skip collision checks if the bullet belongs to the player
			if (gameObject->GetIsPlayer())
			{
				continue;
			}

			CircleCollider* bulletCollider = bullet->GetComponent<CircleCollider>();
			BoxCollider* gameObjectCollider = gameObject->GetComponent<BoxCollider>();

			// Ensure both colliders are valid before checking collision
			if (bulletCollider && gameObjectCollider && bulletCollider->BulletCollision(bullet.get(), gameObject.get()))
			{
				// Mark bullet and object for removal
				if (gameObject->GetIsWall())
				{
					bullet->MarkForRemoval();
				}
				else
				{
					gameObject->MarkForRemoval();
					bullet->MarkForRemoval();
					UpdateScore(50);
					waveKills++;


					//std::cout << "Collision detected between bullet " << bullet << " and " << gameObject->GetName() << std::endl;
				}
			}
		}
	}
}

#pragma endregion


#pragma region ClearingLists
void GameManager::ClearAllLists()
{
	BulletObjectsList.clear();
	GameObjectsList.clear();
	EnemyObjectsList.clear();
}

void GameManager::ClearGameObjectList()
{
	for (auto& gameObject : GetGameObjectList())
	{
		gameObject->MarkForRemoval();
	}
}

void GameManager::ClearEnemiesAndResetPlayer()
{

	for (auto& gameObject : GetGameObjectList())
	{
		// Check if the current object has the tag "Enemy"
		if (gameObject->GetTag() == "Enemy")
		{
			gameObject->MarkForRemoval();
		}
		else if (gameObject->GetTag() == "Player")
		{
			gameObject->SetLocation(500, 450);
		}
	}
}
#pragma endregion 



void GameManager::RemoveMarkedObjectsHelper()
{
	RemoveMarkedObjectsList<Bullet>(GetBulletsList());
	RemoveMarkedObjectsList<GameObject>(GetGameObjectList());
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
			PlayerComponent* playerComponent = gameObject->GetComponent<PlayerComponent>();
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

				if (GetPlayer()->GetIsShooting())
				{
					playerComponent->Shooting();
				}

			}

		}
		else if (gameObject->GetTag() == "Enemy")
		{
			auto enemy = dynamic_cast<Enemy*>(gameObject.get());
			if (enemy)
			{
				enemy->Update(deltaTime, GetPlayer()->GetLocation());
			}
		}
	}

	TextRender(window);
}

#pragma region SpawningEnemies

std::vector<AG::Vector2<float>> GameManager::GenerateRandomSpawnLocations(int numSpawnLocations)
{
	std::vector<AG::Vector2<float>> spawnLocations;
	AG::Vector2<float> playerLocation = GetPlayer()->GetLocation();
	float minDistance = 300.0f;

	// Initialize random number generator
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> disX(80.0f, 850.0f); // Range for X coordinate
	std::uniform_real_distribution<float> disY(80.0f, 850.0f); // Range for Y coordinate

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
			distance = std::sqrt((x - playerLocation.x) * (x - playerLocation.x) + (y - playerLocation.y) * (y - playerLocation.y));

			// Check if the distance is greater than the minimum distance
		} while (distance < minDistance);

		spawnLocations.push_back(AG::Vector2<float>(x, y));
	}

	return spawnLocations;
}

float GameManager::GenerateRandomEnemySpeeds()
{
	std::random_device rd;
	std::mt19937 gen(rd());

	float minSpeed = 8.0f;
	float maxSpeed = 15.0f;

	if (waves >= 10 && waves < 20)
	{
		minSpeed += 1.0f;
		maxSpeed += 1.0f;
	}
	else if (waves >= 20 && waves < 30)
	{
		minSpeed += 2.0f;
		maxSpeed += 2.0f;
	}
	else if (waves >= 30 && waves < 35)
	{
		minSpeed += 3.0f;
		maxSpeed += 3.0f;
	}
	else if (waves >= 40)
	{
		minSpeed += 4.0f;
		maxSpeed += 4.0f;
	}

	std::uniform_real_distribution<float> dist(minSpeed, maxSpeed);
	float generatedSpeed = dist(gen);

	//std::cout << "Randomly generated speed: " << generatedSpeed << std::endl;

	return generatedSpeed;
}

void GameManager::CreateEnemyPool(int numEnemies)
{
	std::cout << "ENEMEIS CREATED: " << numEnemies << std::endl;

	//GetPlayer()->SetLocation(500, 450);
	//GetPlayer()->GetRectangleShape().setPosition(GetPlayer()->GetLocation());

	EnemyObjectsList.clear();

	std::vector<AG::Vector2<float>> spawnLocations = GenerateRandomSpawnLocations(numEnemies);

	for (const auto& spawnLocation : spawnLocations)
	{
		float enemySpeed = GenerateRandomEnemySpeeds();

		// Create a shared pointer to a dynamically allocated Enemy object
		std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>(spawnLocation, enemySpeed);

		AddEnemyObjectsList(std::move(newEnemy));

	}
}

#pragma endregion

#pragma region Player
void GameManager::CreatePlayer()
{
	std::cout << "PLAYER CREATED" << std::endl;

	// Check if a player object already exists
	if (!player)
	{
		// Create a unique pointer to a new player object
		player = std::make_unique<GameObject>();

		player->AddComponent<PlayerComponent>();


		// Add the player object to the GameObjectsList
		AddGameObjectList(player);
	}
}


std::unique_ptr<GameObject>& GameManager::GetPlayer()
{
	for (auto& gameObject : GetGameObjectList())
	{
		// Check if the current object has the tag "Player"
		if (gameObject->GetTag() == "Player")
		{
			// Return a reference to the player object
			return gameObject;
		}
	}

	static std::unique_ptr<GameObject> nullGameObject;
	return nullGameObject;
}

#pragma endregion

#pragma region Walls

void GameManager::CreateWalls()
{

	std::array<std::string, 4> wallNames = { "TopWall", "LeftWall", "BottomWall", "RightWall" };

	for (int i = 0; i < 4; ++i)
	{
		walls[i] = new GameObject();
		AddGameObjectList(walls[i]);
		walls[i]->SetName(wallNames[i]);
		walls[i]->SetTag("Wall");
		walls[i]->AddComponent<BoxCollider>();
	}

	float TopPadding = 20.0f;
	float LeftPadding = 20.0f;
	float BottomPadding = 960.0f;
	float RightPadding = 960.0f;

	//Define the size and locations of the wall
	AG::Vector2<float> VerticalWallSize = AG::Vector2<float>(15, 1000);
	AG::Vector2<float> HorizontalWallSize = AG::Vector2<float>(1000, 15);

	AG::Vector2<float> TopWallPos = AG::Vector2<float>(0.0f, TopPadding);
	AG::Vector2<float> LeftWallPos = AG::Vector2<float>(LeftPadding, 0.0f);
	AG::Vector2<float> BottomWallPos = AG::Vector2<float>(0.0f, BottomPadding);
	AG::Vector2<float> RightWallPos = AG::Vector2<float>(RightPadding, 0.0f);

	//Create a rectangle
	sf::RectangleShape TopWallShape(HorizontalWallSize);
	sf::RectangleShape LeftWallShape(VerticalWallSize);
	sf::RectangleShape BottomWallShape(HorizontalWallSize);
	sf::RectangleShape RightWallShape(VerticalWallSize);

	TopWallShape.setFillColor(sf::Color::Green);
	LeftWallShape.setFillColor(sf::Color::Blue);
	BottomWallShape.setFillColor(sf::Color::Magenta);
	RightWallShape.setFillColor(sf::Color::Yellow);

	TopWallShape.setPosition(TopWallPos);
	LeftWallShape.setPosition(LeftWallPos);
	BottomWallShape.setPosition(BottomWallPos);
	RightWallShape.setPosition(RightWallPos);


	TopWallShape.setOutlineThickness(3);
	LeftWallShape.setOutlineThickness(3);
	BottomWallShape.setOutlineThickness(3);
	RightWallShape.setOutlineThickness(3);

	TopWallShape.setOutlineColor(sf::Color(255, 255, 255));
	LeftWallShape.setOutlineColor(sf::Color(255, 255, 255));
	BottomWallShape.setOutlineColor(sf::Color(255, 255, 255));
	RightWallShape.setOutlineColor(sf::Color(255, 255, 255));

	walls[0]->SetRectangleShape(TopWallShape);
	walls[1]->SetRectangleShape(LeftWallShape);
	walls[2]->SetRectangleShape(BottomWallShape);
	walls[3]->SetRectangleShape(RightWallShape);


	for (int i = 0; i < 4; ++i)
	{
		walls[i]->SetIsWall(true);
	}
}

#pragma endregion


void GameManager::SettingFonts()
{
	//sf::Font font;
	if (!font.loadFromFile("Fonts/PublicPixel.ttf"))
	{
		std::cout << "no font found" << std::endl;
	}

	// Set up score text
	scoreText.setFont(font);
	scoreText.setString("Score: " + std::to_string(playerScore));
	scoreText.setCharacterSize(15);
	//scoreText.setFillColor(sf::Color::Black);
	scoreText.setOutlineColor(sf::Color::Black);
	scoreText.setOutlineThickness(5);
	scoreText.setPosition(55, 52); // Adjust position as needed

	// Set up lives text
	livesText.setFont(font);
	livesText.setString("Lives: " + std::to_string(playerLives));
	livesText.setCharacterSize(15);
	livesText.setOutlineColor(sf::Color::Black);
	livesText.setOutlineThickness(5);
	livesText.setPosition(810, 52); // Adjust position as needed


	// Set up lives text
	wavesText.setFont(font);
	wavesText.setString("Wave: " + std::to_string(waves));
	wavesText.setCharacterSize(15);
	wavesText.setOutlineColor(sf::Color::Black);
	wavesText.setOutlineThickness(5);
	wavesText.setPosition(450, 52); // Adjust position as needed
}

void GameManager::UpdateScore(int points)
{
	playerScore += points;

	scoreText.setString("Score: " + std::to_string(playerScore));

	if (playerScore % 5000 == 0)
	{
		UpdateLives(1, true);
	}
}

void GameManager::UpdateLives(int life, bool increaseLives)
{
	if (increaseLives == true)
	{
		playerLives += life;

		livesText.setString("Lives: " + std::to_string(playerLives));
	}
	else
	{
		playerLives -= life;

		livesText.setString("Lives: " + std::to_string(playerLives));
	}

}

void GameManager::UpdateWaveCounter(int addCount)
{
	if (waveKills == GetEnemyCount())
	{
		waves += addCount;

		wavesText.setString("Wave: " + std::to_string(waves));

		waveKills = 0;
	}
}


int GameManager::GetEnemyCount()
{
	int enemyCount = 0;

	// Iterate through the gameObjectsL list
	for (const auto& gameObject : GetGameObjectList())
	{
		// Check if the game object's tag is "Enemy"
		if (gameObject->GetTag() == "Enemy")
		{
			// Increment the enemy count
			enemyCount++;
		}
	}

	return enemyCount;
}


void GameManager::TextRender(sf::RenderWindow& window)
{
	window.draw(GetLivesTexts());
	window.draw(GetScoreTexts());
	window.draw(wavesText);
}

