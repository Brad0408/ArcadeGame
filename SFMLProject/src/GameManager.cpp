#include "GameManager.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Electrode.h"
#include "Family.h"

//List of object types
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
	//Create unqiue from the raw pointer
	std::unique_ptr<GameObject> gameObjectPtr(gameObject);

	GameObjectsList.push_back(std::move(gameObjectPtr));
}

//Overloads
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
		//Move the objects to the gameobject list
		std::unique_ptr<GameObject> gameObjectPtr = std::move(enemyPtr);

		AddGameObjectList(gameObjectPtr);
	}

	//Clear the original list after moving its elements
	enemyList.clear();
}



//Add to bullet list
void GameManager::AddBulletObjectList(std::unique_ptr<Bullet> bullet)
{
	BulletObjectsList.push_back(std::move(bullet));
}

//Add to enemy list
void GameManager::AddEnemyObjectsList(std::unique_ptr<Enemy> enemy)
{
	EnemyObjectsList.push_back(std::move(enemy));
}


#pragma endregion




#pragma region GettingListNames


//Call this function to see all all the gameobjects, their names, tags and how many of them exist on the list
void GameManager::GetGameObjectListsNames()
{
	std::unordered_map<std::string, int> nameCount;

	for (const auto& gameObjectPtr : GetGameObjectList())
	{
		std::string name = gameObjectPtr->GetName();
		std::string tag = gameObjectPtr->GetTag();

		if (nameCount.find(name) != nameCount.end())
		{
			nameCount[name]++;
		}
		else
		{
			nameCount[name] = 1;
		}

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
	//Object A will always be the player
	GameObject* objectA = nullptr;

	bool isResetEntities = false;
	bool isResetFamily = false;

	//Check for collisions between game objects
	for (auto it = GetGameObjectList().begin(); it != GetGameObjectList().end(); ++it)
	{
		//Set objectA to the player
		if ((*it)->GetTag() == "Player" && !objectA)
		{
			//Convert pointer types
			objectA = it->get();
		}

		for (auto jt = std::next(it); jt != GetGameObjectList().end(); ++jt)
		{
			GameObject* objectB = jt->get();

			//Skip collision checks if both objects are walls
			if (objectA && objectB && objectA->GetTag() == "Wall" && objectB->GetTag() == "Wall")
			{
				continue;
			}

			if (objectA && objectB)
			{
				//Check for specific conditions before collision detection
				if ((objectA->GetTag() == "Enemy" && objectB->GetTag() == "Enemy") || (objectA->GetTag() == "Player" && objectB->GetTag() == "Player"))
				{
					continue; //Skip collision detection between enemies + enemy or between player + player
				}

				BoxCollider* colliderA = objectA->GetComponent<BoxCollider>();
				BoxCollider* colliderB = objectB->GetComponent<BoxCollider>();

				//Ensure both colliders are valid before checking collision
				if (colliderA && colliderB && colliderA->CheckCollision(objectA, objectB))
				{
					//std::cout << "CollisionOccured" << std::endl;


					//Wall Detection
					if (objectA->GetTag() == "Wall" || objectB->GetTag() == "Wall")
					{
						//std::cout << "Wall Collision detected between objects " << objectA->GetName() << " and " << objectB->GetName() << std::endl;

						for (GameObject* wall : GetWalls())
						{
							BoxCollider::WallCollision(objectA, objectB);
						}
					}
					else if(objectA->GetTag() == "Player" && objectB->GetTag() == "Family")
					{
						//std::cout << "hit family" << std::endl;
		
						objectB->MarkForRemoval();

						if (!isResetFamily)
						{
							ResourceManager::PlaySound("FamilySaved");
							UpdateScore(250);
							isResetFamily = true;
						}

					}
					else
					{
						//Reset everything if the player is killed
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
	//Loop over all the currently active bullets
	for (const auto& bullet : GetBulletsList())
	{
		//Loop over all the currently active gameobjects
		for (const auto& gameObject : GetGameObjectList())
		{

			//Family is safe from bullets
			if (gameObject->GetTag() == "Player" || gameObject->GetTag() == "Family")
			{
				continue;
			}

			CircleCollider* bulletCollider = bullet->GetComponent<CircleCollider>();
			BoxCollider* gameObjectCollider = gameObject->GetComponent<BoxCollider>();

			if (bulletCollider && gameObjectCollider && bulletCollider->BulletCollision(bullet.get(), gameObject.get()))
			{
				//Destroy the bullet if it hits a wall or electrode
				if (gameObject->GetTag() == "Wall" || gameObject->GetTag() == "Electrode")
				{
					bullet->MarkForRemoval();
				}
				else
				{
					//Destroy what ever the bullet hit
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

void GameManager::ClearAndResetEntites()
{
	//Tags to remove
	std::vector<std::string> tagsToRemove = { "Enemy", "Family", "Electrode" };

	for (auto& gameObject : GetGameObjectList())
	{
		std::string tag = gameObject->GetTag();

		if (std::find(tagsToRemove.begin(), tagsToRemove.end(), tag) != tagsToRemove.end())
		{
			//Remove everything
			gameObject->MarkForRemoval();
		}
		else if (tag == "Player")  //Just reset the player back to the middle, deleting and recreating the creates 2 instances for some reason
		{
			gameObject->SetLocation(500, 450);
			gameObject->GetRectangleShape().setTextureRect(sf::IntRect(342, 164, 24, 24));
			gameObject->SetRectangleShape(gameObject->GetRectangleShape());
		}
	}
}

//Remove bullets for when the game over screen is shown
void GameManager::ClearAnyBullets()
{
	for (auto& gameObject : GetBulletsList())
	{
		if (gameObject->GetTag() == "Bullet")
		{
			//std::cout << "REMOVE BULELTS" << std::endl;
			gameObject->MarkForRemoval();
		}
	}
}
#pragma endregion 


//Template function in header being called to remove all currently marked bullets and objects
void GameManager::RemoveMarkedObjectsHelper()
{
	RemoveMarkedObjectsList<Bullet>(GetBulletsList());
	RemoveMarkedObjectsList<GameObject>(GetGameObjectList());
}




void GameManager::Update(float deltaTime, sf::RenderWindow& window)
{
	for (auto& gameObject : GetGameObjectList())
	{
		//Player Update
		if (gameObject->GetTag() == "Player")
		{
			PlayerComponent* playerComponent = gameObject->GetComponent<PlayerComponent>();
			if (playerComponent)
			{
				playerComponent->Update(deltaTime);

				//Stop drawing the firing point and stop the player from shooting when the game is over
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
		//Bullet Update
		for (auto& bullet : GetBulletsList())
		{
			if (bullet)
			{
				bullet->Update(deltaTime);

				//Want outlines to show - prefer how it looks
				bullet->GetComponent<CircleCollider>()->DrawOutlines(bullet->GetCircleShape());
			}
		}

		for (auto& gameObject : GetGameObjectList())
		{
			//Enemy Update
			if (gameObject->GetTag() == "Enemy")
			{
				auto enemy = dynamic_cast<Enemy*>(gameObject.get());
				if (enemy)
				{
					enemy->Update(deltaTime, GetPlayer()->GetLocation());
				}
			}
			//Family Update
			else if (gameObject->GetTag() == "Family")
			{
				auto family = dynamic_cast<Family*>(gameObject.get());
				if (family)
				{
					family->Update(deltaTime);
				}
			}
			//Electrode Update
			else if (gameObject->GetTag() == "Electrode")
			{
				auto electrode = dynamic_cast<Electrode*>(gameObject.get());
				if (electrode)
				{
					electrode->Update(deltaTime);
				}
			}
		}


		GenericCollision();
		BulletCollisions();



		bool enemiesExist = false;

		//Iterate over the game objects to check for enemies
		for (const auto& gameObject : GetGameObjectList())
		{
			if (gameObject->GetTag() == "Enemy")
			{
				//At least one enemy is found - so set it to true
				enemiesExist = true;
				break;
			}
		}


		//If no enemies are found
		if (!enemiesExist)
		{
			ResetElectrodes();

			//Recreate the enemeis - Decide how many to spawn based on the wave count
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
			
			//Everything killed = +1 wave
			UpdateWaveCounter(1);
		}
	}
	else
	{
		GameOver();
	}

	//This renders across all screen, do outside the game loop check
	TextRender(window);
}

#pragma region SpawningEnemiesAndFamily

std::vector<AG::Vector2<float>> GameManager::GenerateRandomSpawnLocations(int numSpawnLocations)
{
	std::vector<AG::Vector2<float>> spawnLocations;
	AG::Vector2<float> playerLocation = GetPlayer()->GetLocation();
	float minDistance = 300.0f;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> disX(80.0f, 850.0f); //Range for X coordinate
	std::uniform_real_distribution<float> disY(80.0f, 850.0f); //Range for Y coordinate

	//Generate random spawn locations
	for (int i = 0; i < numSpawnLocations; ++i)
	{
		float x, y;
		float distance;

		do
		{
			//Generate random coordinates
			x = disX(gen);
			y = disY(gen);

			//Calculate distance between player spawn location and current spawn location
			distance = std::sqrt((x - playerLocation.x) * (x - playerLocation.x) + (y - playerLocation.y) * (y - playerLocation.y));
			

			//Check if the distance is greater than the minimum distance
		} while (distance < minDistance);

		spawnLocations.push_back(AG::Vector2<float>(x, y));
	}

	return spawnLocations;
}

//Generate random speeds based of the wave count
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

//Reset electrode because the player can destroy them
void GameManager::ResetElectrodes()
{
	for (const auto& gameObject : GetGameObjectList())
	{
		if (gameObject->GetTag() == "Electrode")
		{
			gameObject->MarkForRemoval();
		}
	}
}

//Creates enemies
void GameManager::CreateEnemyPool(int numEnemies)
{
	std::cout << "Enemies created: " << numEnemies << std::endl;

	//GetPlayer()->SetLocation(500, 450);
	//GetPlayer()->GetRectangleShape().setPosition(GetPlayer()->GetLocation());

	EnemyObjectsList.clear();

	std::vector<AG::Vector2<float>> spawnLocations = GenerateRandomSpawnLocations(numEnemies);
	std::vector<AG::Vector2<float>> electrodeSpawnLocations = GenerateRandomSpawnLocations(7);

	//Creating grunts
	for (const auto& spawnLocation : spawnLocations)
	{
		float enemySpeed = GenerateRandomEnemySpeeds();

		// Create a shared pointer to a dynamically allocated Enemy object
		std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>(spawnLocation, enemySpeed);

		AddEnemyObjectsList(std::move(newEnemy));

	}

	//Creating Electrodes
	for (const auto& spawnLocation : electrodeSpawnLocations)
	{
		// Create a shared pointer to a dynamically allocated Enemy object
		std::unique_ptr<Electrode> newElectrode = std::make_unique<Electrode>(spawnLocation, 0);

		AddEnemyObjectsList(std::move(newElectrode));
	}
	
}

//Creating family
void GameManager::CreateFamily(int numFamilies)
{
	std::cout << "Family created: " << numFamilies << std::endl;

	//Because family wont be killed by the player - clean the exiting ones of the screen when +1 wave
	GetGameObjectList().remove_if([](const std::unique_ptr<GameObject>& obj)
	{
		//std::cout << "REWMOVE FAIUMTYIL "  << std::endl;
		return obj->GetTag() == "Family";
	});

	std::vector<AG::Vector2<float>> spawnLocations = GenerateRandomSpawnLocations(numFamilies);

	//Create/Recreate family 
	for (const auto& spawnLocation : spawnLocations)
	{
		std::unique_ptr<Family> newFamily = std::make_unique<Family>(spawnLocation);

		AddGameObjectList(newFamily);
	}
}

#pragma endregion

#pragma region Player

//Create player
void GameManager::CreatePlayer()
{
	std::cout << "PLAYER CREATED" << std::endl;

	//Check if a player object already exists
	if (!player)
	{
		player = std::make_unique<GameObject>();

		player->AddComponent<PlayerComponent>();

		AddGameObjectList(player);
	}
	else
	{
		std::cout << "already a player" << std::endl;
	}
}

//Gets reference to the player
std::unique_ptr<GameObject>& GameManager::GetPlayer()
{
	for (auto& gameObject : GetGameObjectList())
	{
		if (gameObject->GetTag() == "Player")
		{
			return gameObject;
		}
	}

	static std::unique_ptr<GameObject> nullGameObject;
	return nullGameObject;
}

#pragma endregion

#pragma region Walls

//Create walls
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

	//Location and sizes
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

//Loads font - Also sets each text to that font and other sets
void GameManager::SettingFont()
{
	if (!font.loadFromFile("Fonts/PublicPixel.ttf"))
	{
		std::cout << "no font found" << std::endl;
	}

	//Set up score text
	scoreText.setFont(font);
	scoreText.setString("Score: " + std::to_string(playerScore));
	scoreText.setCharacterSize(15);
	scoreText.setOutlineColor(sf::Color::Black);
	scoreText.setOutlineThickness(5);
	scoreText.setPosition(55, 52);

	//Set up lives text
	livesText.setFont(font);
	livesText.setString("Lives: " + std::to_string(playerLives));
	livesText.setCharacterSize(15);
	livesText.setOutlineColor(sf::Color::Black);
	livesText.setOutlineThickness(5);
	livesText.setPosition(810, 52);


	//Set up waves text
	wavesText.setFont(font);
	wavesText.setString("Wave: " + std::to_string(waves));
	wavesText.setCharacterSize(15);
	wavesText.setOutlineColor(sf::Color::Black);
	wavesText.setOutlineThickness(5);
	wavesText.setPosition(450, 52);

	//////////////////////Game Over Text/////////////////////

	//Set up gameover text
	gameOverText.setFont(font);
	gameOverText.setString("Game Over !");
	gameOverText.setCharacterSize(50);
	gameOverText.setOutlineColor(sf::Color::Black);
	gameOverText.setOutlineThickness(5);
	gameOverText.setPosition(230, 100);

	//Set the highscore text
	hightScoreText.setFont(font);
	hightScoreText.setCharacterSize(15);
	hightScoreText.setOutlineColor(sf::Color::Black);
	hightScoreText.setOutlineThickness(5);
	hightScoreText.setPosition(620, 300);

	//Set the highest wave text
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

//Main menu title screen
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
	//Change the position of the score and wave text
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

	for (const auto& gameObject : GetGameObjectList())
	{
		if (gameObject->GetTag() == "Enemy")
		{
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
	ResourceManager::StopMusic(); //Stop main menu music
	ResourceManager::PlayMusic("Gameplay");
	gameStarted = true;
	CreatePlayer();
	CreateWalls();
	CreateFamily(5);
	SettingGameplayText();

	CreateEnemyPool(30);
	AddGameObjectList(GameManager::GetEnemyList());
}

void GameManager::RestartGame()
{
	std::cout << "Restart Game " << std::endl;

	ResourceManager::PlaySound("GameStart");
	ResourceManager::StopMusic(); //Stop game over music
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

	//Set the highscores and highest waves
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
	//Shifts the location of the text from gameplay to the new locations for the game over screen
	scoreText.setString("Your Score: " + std::to_string(playerScore));
	scoreText.setPosition(185, 300);

	wavesText.setString("Your Wave: " + std::to_string(waves));
	wavesText.setPosition(185, 330);


	hightScoreText.setString("High Score: " + std::to_string(highScore));
	highWaveText.setString("Highest Wave: " + std::to_string(highWaveScore));
	playthegameText.setPosition(340, 200);
}

