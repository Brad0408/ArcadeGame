#include <Event.h>
#include <Bullet.h>

#define FIXEDFRAMERATE (1.f/60.f)






int main()
{
	//Window
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "Robotron 2084");


	//deltaTime declare
	std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
	float deltaTime = 0.0f;
	float timeSincePhysicsStep = 0.0f;

	//Creating the player sprite for the main menu
	sf::RectangleShape mainMenuPlayer;
	mainMenuPlayer.setOutlineThickness(3);
	sf::IntRect mainMenuPlayerTextureUV = sf::IntRect(342, 164, 24, 24);

	mainMenuPlayer.setSize(AG::Vector2<float>::one * 35);
	mainMenuPlayer.setTexture(&ResourceManager::GetTexture("Player"));
	mainMenuPlayer.setTextureRect(mainMenuPlayerTextureUV);
	mainMenuPlayer.setOrigin(AG::Vector2<float>::one * 35 / 2);
	mainMenuPlayer.setPosition(500, 450);

	//Check for loading from main menu to gameplay
	bool spaceKeyPressed = false;



	GameManager::SettingFont();
	ResourceManager::CreateSoundBuffers();
	ResourceManager::CreateMusicTracks();
	ResourceManager::PlayMusic("MainMenu");


	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !spaceKeyPressed)
			{
				//Start the game once
				GameManager::StartGame();
				spaceKeyPressed = true;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && spaceKeyPressed && GameManager::IsGameOver())
			{
				//Restart the game if the game is over
				GameManager::RestartGame();
			}

			//Do player inputs here, for some reason the player would continue to shoot even if the mouse was released, pasing in event as paramater must not work well
			if (GameManager::IsGameStarted() && !GameManager::IsGameOver())
			{
				//Left click to shoot
				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
				{
					GameManager::GetPlayer()->SetIsShooting(true);
				}

				//Release to stop shooting
				if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
				{
					GameManager::GetPlayer()->SetIsShooting(false);
				}
			}

		}

		//delaTime calculate
		std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
		deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - lastTime).count() / 100000.0f;
		lastTime = now;




		





		timeSincePhysicsStep += deltaTime;
		while (timeSincePhysicsStep > FIXEDFRAMERATE)
		{
			//Game runs extremely slow when these are called in the fixed frame frate - doing through update instead
			//GameManager::GenericCollision();
			//GameManager::BulletCollisions();

			timeSincePhysicsStep -= FIXEDFRAMERATE;
		}




		//Clear
		window.clear();


		if (GameManager::IsGameStarted())
		{
		
			/////////////Rendering////////////////

			for (const auto& gameObjectPtr : GameManager::GetGameObjectList())
			{
				window.draw(gameObjectPtr->GetRectangleShape());
			}

			for (const auto& bullObjectPtr : GameManager::GetBulletsList())
			{
				window.draw(bullObjectPtr->GetCircleShape());
			}



			///////////Updating////////////////

			GameManager::Update(deltaTime, window);
			GameManager::RemoveMarkedObjectsHelper();

		}
		else
		{
			//Draw the main menu when the game is not active
			window.draw(GameManager::MainMenuText());
			window.draw(GameManager::MainMenuRobotronText());
			window.draw(mainMenuPlayer);
		}


		//Display whats actually been rendered
		window.display();
	}

	//Memory Cleanup At Terimination
	ResourceManager::ClearTextureMap();
	GameManager::ClearFont();

	//Needed in Release mode
	ResourceManager::AudioCleanup();


	return 0;
}
