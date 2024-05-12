#include <Event.h>
#include <Bullet.h>


#define FIXEDFRAMERATE 0.025f




int main()
{

	GameManager::CreatePlayer();
	GameManager::CreateWalls();
	GameManager::CreateEnemyPool(20);
	GameManager::SettingFonts();

	//Put newly made gameObjects on the List
	GameManager::AddGameObjectList(GameManager::GetEnemyList());




	//Create window of resolution
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "Robotron 2024");



	//Get the time before getting into the loop
	std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
	float deltaTime = 0.0f;
	float timeSincePhysicsStep = 0.0f;


	//Test shape
	//sf::RectangleShape TestShape(sf::Vector2f(100, 50));
	//TestShape.setFillColor(sf::Color::Green);
	//TestShape.setPosition(200, 200);
	




	std::list<std::unique_ptr<GameObject>>& gameObjectsL = GameManager::GetGameObjectList();
	std::list<std::unique_ptr<Bullet>>& bulletObjectsL = GameManager::GetBulletsList();
	//GameManager::GetGameObjectListsNames(GameManager::GetGameObjectList());

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	while (window.isOpen())
	{

		//Declare event
		sf::Event event;

		while (window.pollEvent(event))
		{
			//if the event type if closing the window - close the window
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
		deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - lastTime).count() / 100000.0f;
		lastTime = now;




		GameManager::GenericCollision();
		GameManager::BulletCollisions();

	
		
		bool enemiesExist = false;

		// Iterate over the game objects to check for enemies
		for (const auto& gameObject : gameObjectsL)
		{
			if (gameObject->GetTag() == "Enemy")
			{
				// At least one enemy is found
				enemiesExist = true;
				break;
			}
		}

		int wave = GameManager::GetWaveCount();

		// Check if no enemies are found
		if (!enemiesExist)
		{
			if (wave < 10)
			{
				GameManager::CreateEnemyPool(20);
				GameManager::AddGameObjectList(GameManager::GetEnemyList());
			}
			else if (wave >= 10 && wave < 20)
			{
				GameManager::CreateEnemyPool(25);
				GameManager::AddGameObjectList(GameManager::GetEnemyList());
			}
			else if (wave >= 20 && wave < 30)
			{
				GameManager::CreateEnemyPool(30);
				GameManager::AddGameObjectList(GameManager::GetEnemyList());
			}
			else if (wave >= 30 && wave < 35)
			{
				GameManager::CreateEnemyPool(35);
				GameManager::AddGameObjectList(GameManager::GetEnemyList());
			}
			else if (wave >= 40)
			{
				GameManager::CreateEnemyPool(40);
				GameManager::AddGameObjectList(GameManager::GetEnemyList());
			}



			GameManager::UpdateWaveCounter(1);
		}

		

		//sf::SoundBuffer buffer;





		timeSincePhysicsStep += deltaTime;
		while (timeSincePhysicsStep > FIXEDFRAMERATE)
		{
			timeSincePhysicsStep -= FIXEDFRAMERATE;
		}




		//Clear
		window.clear();


		/////////////Rendering////////////////

		for (const auto& gameObjectPtr : gameObjectsL)
		{
			window.draw(gameObjectPtr->GetRectangleShape());
		}

		for (const auto& bullObjectPtr : bulletObjectsL)
		{
			window.draw(bullObjectPtr->GetCircleShape());
		}






		///////////Updating////////////////

		GameManager::Update(deltaTime, window, event);

		GameManager::RemoveMarkedObjectsHelper();



		//Display whats actually been rendered
		window.display();


	}

	//Cleanup At Terimination
	ResourceManager::ClearTextureMap();
	//ResourceManager::ClearSoundBufferMap();

	GameManager::ClearFont();
	GameManager::ClearAllLists();


	return 0;
}
