#include <Event.h>
#include <Bullet.h>


#define FIXEDFRAMERATE 0.025f




int main()
{
	//Window
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "Robotron 2024");


	//deltaTime
	std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
	float deltaTime = 0.0f;
	float timeSincePhysicsStep = 0.0f;




	bool spaceKeyPressed = false;

	GameManager::SettingFont();

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


		}

		std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
		deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - lastTime).count() / 100000.0f;
		lastTime = now;




		

		//sf::SoundBuffer buffer;





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

			GameManager::Update(deltaTime, window, event);

			GameManager::RemoveMarkedObjectsHelper();

		}
		else
		{
			window.draw(GameManager::MainMenuText());
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
