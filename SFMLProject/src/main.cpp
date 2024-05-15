#include <Event.h>
#include <Bullet.h>




#define FIXEDFRAMERATE 0.025f






int main()
{
	//Window
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "Robotron 2084");


	//deltaTime
	std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
	float deltaTime = 0.0f;
	float timeSincePhysicsStep = 0.0f;


	sf::RectangleShape mainMenuPlayer;
	sf::IntRect mainMenuPlayerTextureUV = sf::IntRect(342, 164, 24, 24);

	mainMenuPlayer.setSize(AG::Vector2<float>::one * 35);
	mainMenuPlayer.setTexture(&ResourceManager::GetTexture("Player"));
	mainMenuPlayer.setTextureRect(mainMenuPlayerTextureUV);
	mainMenuPlayer.setOrigin(AG::Vector2<float>::one * 35 / 2);
	mainMenuPlayer.setPosition(500, 450);

	bool spaceKeyPressed = false;

	GameManager::SettingFont();





	//sf::SoundBuffer buffer;
	//if (!buffer.loadFromFile("Audio/Shoot.wav"))
	//{
	//	std::cout << "nsdoinbasdas no sound" << std::endl;
	//}

	//sf::Sound sound;

	//sound.setBuffer(buffer);
	//sound.play();




	ResourceManager::CreateSoundBuffers();
	ResourceManager::CreateMusicTracks();
	ResourceManager::PlayMusic("MainMenu");

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
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !spaceKeyPressed)
			{
				GameManager::StartGame();
				spaceKeyPressed = true; // Set the flag to true to indicate that the space key has been pressed
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && spaceKeyPressed && GameManager::IsGameOver())
			{
				GameManager::RestartGame();
			}

			//Do player inputs here, for some reason the player would continue to shoot even if the mouse was released, pasing in event as paramater must cause some lag issues
			if (GameManager::IsGameStarted() && !GameManager::IsGameOver())
			{
				// Check for left mouse button press event
				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
				{
					GameManager::GetPlayer()->SetIsShooting(true);
				}

				// Check for left mouse button release event
				if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
				{
					GameManager::GetPlayer()->SetIsShooting(false);
				}
			}

		}

		std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
		deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - lastTime).count() / 100000.0f;
		lastTime = now;




		





		timeSincePhysicsStep += deltaTime;
		while (timeSincePhysicsStep > FIXEDFRAMERATE)
		{
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
			window.draw(GameManager::MainMenuText());
			window.draw(GameManager::MainMenuRobotronText());
			window.draw(mainMenuPlayer);
		}




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
