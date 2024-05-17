#include "GameManager.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Family.h"

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
sf::Text GameManager::gameOverText;
sf::Text GameManager::playthegameText;
sf::Text GameManager::robotronText;
sf::Text GameManager::hightScoreText;
sf::Text GameManager::highWaveText;

int GameManager::playerScore = 0;
int GameManager::playerLives = 5;
int GameManager::highScore = 0;
int GameManager::highWaveScore = 0;
int GameManager::waveKills = 0;
int GameManager::waves = 0;
bool GameManager::gameOver = false;
bool GameManager::gameStarted = false;



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

void GameManager::AddGameObjectList(std::unique_ptr<Family> &gameObject)
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

	bool isResetEntities = false;
	bool isResetFamily = false;

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
			if (objectA && objectB && objectA->GetTag() == "Wall" && objectB->GetTag() == "Wall")
			{
				continue;
			}

			// Continue with your collision detection logic here
			if (objectA && objectB)
			{
				// Check for specific conditions before collision detection
				if ((objectA->GetTag() == "Enemy" && objectB->GetTag() == "Enemy") || (objectA->GetTag() == "Player" && objectB->GetTag() == "Player"))
				{
					continue; // Skip collision detection between enemies or between players
				}

				BoxCollider* colliderA = objectA->GetComponent<BoxCollider>();
				BoxCollider* colliderB = objectB->GetComponent<BoxCollider>();

				// Ensure both colliders are valid before checking collision
				if (colliderA && colliderB && colliderA->CheckCollision(objectA, objectB))
				{
					//std::cout << "CollisionOccured" << std::endl;


					// Wall Detection
					if (objectA->GetTag() == "Wall" || objectB->GetTag() == "Wall")
					{
						//std::cout << "Wall Collision detected between objects " << objectA->GetName() << " and " << objectB->GetName() << std::endl;

						// Handle wall collision
						for (GameObject* wall : GetWalls())
						{
							BoxCollider::WallCollision(objectA, objectB);
						}
					}
					else if(objectA->GetTag() == "Player" && objectB->GetTag() == "Family")
					{
						std::cout << "hit family" << std::endl;
						objectB->MarkForRemoval();

						if (!isResetFamily)
						{
							UpdateScore(250);
							isResetFamily = true;
						}

					}
					else
					{
						// Handle collision between non-wall objects
						if (!isResetEntities)
						{
							ResourceManager::PlaySound("Respawn");
							ClearAndResetEntites();
							UpdateLives(1, false);
							isResetEntities = true;
							waveKills = 0;
							CreateFamily(5);
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
			if (gameObject->GetTag() == "Player" || gameObject->GetTag() == "Family")
			{
				continue;
			}

			CircleCollider* bulletCollider = bullet->GetComponent<CircleCollider>();
			BoxCollider* gameObjectCollider = gameObject->GetComponent<BoxCollider>();

			// Ensure both colliders are valid before checking collision
			if (bulletCollider && gameObjectCollider && bulletCollider->BulletCollision(bullet.get(), gameObject.get()))
			{
				// Mark bullet and object for removal
				if (gameObject->GetTag() == "Wall")
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

void GameManager::ClearAndResetEntites()
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
			gameObject->GetRectangleShape().setTextureRect(sf::IntRect(342, 164, 24, 24));
			gameObject->SetRectangleShape(gameObject->GetRectangleShape());
		}
		else if (gameObject->GetTag() == "Family")
		{
			gameObject->MarkForRemoval();
		}
	}
}
void GameManager::ClearAnyBullets()
{
	for (auto& gameObject : GetBulletsList())
	{
		if (gameObject->GetTag() == "Bullet")
		{
			std::cout << "REMOVE BULELTS" << std::endl;
			gameObject->MarkForRemoval();
		}
	}
}
#pragma endregion 



void GameManager::RemoveMarkedObjectsHelper()
{
	RemoveMarkedObjectsList<Bullet>(GetBulletsList());
	RemoveMarkedObjectsList<GameObject>(GetGameObjectList());
}




void GameManager::Update(float deltaTime, sf::RenderWindow& window)
{
	for (auto& gameObject : GetGameObjectList())
	{
		// Check if the current object has the tag "Player"
		if (gameObject->GetTag() == "Player")
		{
			PlayerComponent* playerComponent = gameObject->GetComponent<PlayerComponent>();
			if (playerComponent)
			{
				playerComponent->Update(deltaTime);

				if (!gameOver)
				{
					playerComponent->CalculateFiringPointRotation(window);


					if (GetPlayer()->GetIsShooting())
					{
						playerComponent->Shooting();
					}
				}
			}
		}
	}


	if (!gameOver)
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
			if (gameObject->GetTag() == "Enemy")
			{
				auto enemy = dynamic_cast<Enemy*>(gameObject.get());
				if (enemy)
				{
					enemy->Update(deltaTime, GetPlayer()->GetLocation());
				}
			}
			else if (gameObject->GetTag() == "Family")
			{
				auto family = dynamic_cast<Family*>(gameObject.get());
				if (family)
				{
					family->Update(deltaTime);
				}
			}
		}


		GenericCollision();
		BulletCollisions();



		bool enemiesExist = false;

		// Iterate over the game objects to check for enemies
		for (const auto& gameObject : GetGameObjectList())
		{
			if (gameObject->GetTag() == "Enemy")
			{
				// At least one enemy is found
				enemiesExist = true;
				break;
			}
		}


		// Check if no enemies are found
		if (!enemiesExist)
		{
			if (waves <= 10)
			{
				CreateEnemyPool(30);
				AddGameObjectList(GetEnemyList());
				CreateFamily(5);
			}
			else if (waves >= 10 && waves < 20)
			{
				CreateEnemyPool(35);
				AddGameObjectList(GetEnemyList());
				CreateFamily(5);
			}
			else if (waves >= 20 && waves < 30)
			{
				CreateEnemyPool(40);
				AddGameObjectList(GetEnemyList());
				CreateFamily(5);
			}
			else if (waves >= 30 && waves < 35)
			{
				CreateEnemyPool(45);
				AddGameObjectList(GetEnemyList());
				CreateFamily(5);
			}
			else if (waves >= 40)
			{
				CreateEnemyPool(55);
				AddGameObjectList(GetEnemyList());
				CreateFamily(5);
			}



			UpdateWaveCounter(1);
		}



	}
	else
	{
		GameOver();
	}


	TextRender(window);

}

#pragma region SpawningEnemiesAndFamily

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
	float maxSpeed = 16.0f;

	if (waves >= 10 && waves < 20)
	{
		minSpeed += 1.0f;
		maxSpeed += 1.0f;
	}
	else if (waves >= 20 && waves < 30)
	{
		minSpeed += 2.5f;
		maxSpeed += 2.5f;
	}
	else if (waves >= 30 && waves < 35)
	{
		minSpeed += 4.0f;
		maxSpeed += 4.0f;
	}
	else if (waves >= 40)
	{
		minSpeed += 5.5f;
		maxSpeed += 5.5f;
	}

	std::uniform_real_distribution<float> dist(minSpeed, maxSpeed);
	float generatedSpeed = dist(gen);

	//std::cout << "Randomly generated speed: " << generatedSpeed << std::endl;

	return generatedSpeed;
}

void GameManager::CreateEnemyPool(int numEnemies)
{
	std::cout << "Enemies created: " << numEnemies << std::endl;

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


void GameManager::CreateFamily(int numFamilies)
{
	std::cout << "Family created: " << numFamilies << std::endl;

	GetGameObjectList().remove_if([](const std::unique_ptr<GameObject>& obj)
	{
		//std::cout << "REWMOVE FAIUMTYIL created: "  << std::endl;
		return obj->GetTag() == "Family";
	});

	std::vector<AG::Vector2<float>> spawnLocations = GenerateRandomSpawnLocations(numFamilies);

	for (const auto& spawnLocation : spawnLocations)
	{
		// Create a shared pointer to a dynamically allocated Enemy object
		std::unique_ptr<Family> newFamily = std::make_unique<Family>(spawnLocation);

		AddGameObjectList(newFamily);
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
	else
	{
		std::cout << "already a aplayer" << std::endl;
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

	TopWallShape.setFillColor(sf::Color::Red);
	LeftWallShape.setFillColor(sf::Color::Red);
	BottomWallShape.setFillColor(sf::Color::Red);
	RightWallShape.setFillColor(sf::Color::Red);

	TopWallShape.setPosition(TopWallPos);
	LeftWallShape.setPosition(LeftWallPos);
	BottomWallShape.setPosition(BottomWallPos);
	RightWallShape.setPosition(RightWallPos);


	TopWallShape.setOutlineThickness(2);
	LeftWallShape.setOutlineThickness(2);
	BottomWallShape.setOutlineThickness(2);
	RightWallShape.setOutlineThickness(2);

	walls[0]->SetRectangleShape(TopWallShape);
	walls[1]->SetRectangleShape(LeftWallShape);
	walls[2]->SetRectangleShape(BottomWallShape);
	walls[3]->SetRectangleShape(RightWallShape);

}


#pragma endregion


#pragma region FontsAndText

void GameManager::SettingFont()
{
	if (!font.loadFromFile("Fonts/PublicPixel.ttf"))
	{
		std::cout << "no font found" << std::endl;
	}

	// Set up score text
	scoreText.setFont(font);
	scoreText.setString("Score: " + std::to_string(playerScore));
	scoreText.setCharacterSize(15);
	scoreText.setOutlineColor(sf::Color::Black);
	scoreText.setOutlineThickness(5);
	scoreText.setPosition(55, 52);

	// Set up lives text
	livesText.setFont(font);
	livesText.setString("Lives: " + std::to_string(playerLives));
	livesText.setCharacterSize(15);
	livesText.setOutlineColor(sf::Color::Black);
	livesText.setOutlineThickness(5);
	livesText.setPosition(810, 52);


	// Set up lives text
	wavesText.setFont(font);
	wavesText.setString("Wave: " + std::to_string(waves));
	wavesText.setCharacterSize(15);
	wavesText.setOutlineColor(sf::Color::Black);
	wavesText.setOutlineThickness(5);
	wavesText.setPosition(450, 52);





	//////////////////////Game Over Text/////////////////////

	gameOverText.setFont(font);
	gameOverText.setString("Game Over !");
	gameOverText.setCharacterSize(50);
	gameOverText.setOutlineColor(sf::Color::Black);
	gameOverText.setOutlineThickness(5);
	gameOverText.setPosition(230, 100);

	hightScoreText.setFont(font);
	hightScoreText.setCharacterSize(15);
	hightScoreText.setOutlineColor(sf::Color::Black);
	hightScoreText.setOutlineThickness(5);
	hightScoreText.setPosition(620, 300);

	highWaveText.setFont(font);
	highWaveText.setCharacterSize(15);
	highWaveText.setOutlineColor(sf::Color::Black);
	highWaveText.setOutlineThickness(5);
	highWaveText.setPosition(620, 330);
}

sf::Text &GameManager::MainMenuText()
{
	playthegameText.setFont(font);
	playthegameText.setString("Press 'Space' To Play");
	playthegameText.setCharacterSize(15);
	playthegameText.setOutlineColor(sf::Color::Black);
	playthegameText.setOutlineThickness(5);
	playthegameText.setPosition(340, 200);

	return playthegameText;
}

sf::Text& GameManager::MainMenuRobotronText()
{
	robotronText.setFont(font);
	robotronText.setString("Robotron 2084");
	robotronText.setCharacterSize(50);
	robotronText.setOutlineColor(sf::Color::Black);
	robotronText.setOutlineThickness(5);
	robotronText.setPosition(190, 100);

	return robotronText;
}

void GameManager::SettingGameplayText()
{
	scoreText.setPosition(55, 52);
	wavesText.setPosition(450, 52);

	scoreText.setString("Score: " + std::to_string(playerScore)); 
	wavesText.setString("Wave: " + std::to_string(waves));
	livesText.setString("Lives: " + std::to_string(playerLives));
}


#pragma endregion


#pragma region UpdateGameplayTexts

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
		if (playerLives < 5)
		{
			ResourceManager::PlaySound("LivesUp");

			playerLives += life;

			livesText.setString("Lives: " + std::to_string(playerLives));
		}

	}
	else
	{
		playerLives -= life;

		livesText.setString("Lives: " + std::to_string(playerLives));

		if (playerLives <= 0)
		{
			ResourceManager::StopMusic();
			ResourceManager::PlayMusic("GameOver");

			playerLives = 0;
			livesText.setString("Lives: " + std::to_string(playerLives));
			gameOver = true;
		}
	}

}

void GameManager::UpdateWaveCounter(int addCount)
{
	//std::cout << "Kill count: " << waveKills << std::endl;
	//std::cout << "Enemy count: " <<GetEnemyCount() <<std::endl;

	if (waveKills >= GetEnemyCount())
	{
		ResourceManager::PlaySound("WaveUp");

		waves += addCount;

		wavesText.setString("Wave: " + std::to_string(waves));

		waveKills = 0;
	}
}

#pragma endregion

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
	if (!gameOver)
	{
		//Drawing Gameplay text
		window.draw(GetLivesTexts());
		window.draw(GetScoreTexts());
		window.draw(wavesText);
	}
	else
	{
		//Drawing Gameover text
		window.draw(gameOverText);
		window.draw(GetScoreTexts());
		window.draw(wavesText);
		window.draw(playthegameText);
		window.draw(hightScoreText);
		window.draw(highWaveText);
	}
}

void GameManager::StartGame()
{
	std::cout << "GAME START" << std::endl;

	ResourceManager::PlaySound("GameStart");
	ResourceManager::StopMusic();
	ResourceManager::PlayMusic("Gameplay");
	gameStarted = true;
	CreatePlayer();
	CreateWalls();
	CreateEnemyPool(30);
	CreateFamily(5);
	SettingGameplayText();

	AddGameObjectList(GameManager::GetEnemyList());
	
}

void GameManager::RestartGame()
{
	std::cout << "restart game " << std::endl;

	ResourceManager::PlaySound("GameStart");

	ResourceManager::StopMusic();
	ResourceManager::PlayMusic("Gameplay");

	GetPlayer()->SetIsShooting(false);
	CreateEnemyPool(30);
	AddGameObjectList(GameManager::GetEnemyList());
	CreateFamily(5);
	gameOver = false;
	waves = 0;
	playerScore = 0;
	playerLives = 5;


	SettingGameplayText();
}

void GameManager::GameOver()
{
	ClearAndResetEntites();
	ClearAnyBullets();
	SetGameOverScreen();
	ResourceManager::StopSounds();


	if (highScore < playerScore)
	{
		highScore = playerScore;
	}


	if (highWaveScore < waves)
	{
		highWaveScore = waves;
	}
}

void GameManager::SetGameOverScreen()
{

	scoreText.setString("Your Score: " + std::to_string(playerScore));
	scoreText.setPosition(185, 300);

	wavesText.setString("Your Wave: " + std::to_string(waves));
	wavesText.setPosition(185, 330);


	hightScoreText.setString("High Score: " + std::to_string(highScore));
	highWaveText.setString("Highest Wave: " + std::to_string(highWaveScore));
	playthegameText.setPosition(340, 200);
}

