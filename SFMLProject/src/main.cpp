#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <Vector2.h>
#include <Event.h>


#define FIXEDFRAMERATE 0.025f

class TestB
{

public:
	AG::Event<int> OnSomething;

	void BroadcastOnSomething(int arg1)
	{
		OnSomething(arg1);
	}
};

class TestA
{

private:
	TestB* other;

public:
	TestA(TestB* _other) : other(_other) { other->OnSomething += std::bind(&TestA::Handle_ThangSomething, this, std::placeholders::_1); }

	void Handle_ThangSomething(int in)
	{
		std::cout << in << std::endl;
		other->OnSomething -= std::bind(&TestA::Handle_ThangSomething, this, std::placeholders::_1);
	}

	void OnSomething();


};




int main()
{
	TestB a;
	TestA b(&a);

	

	//Create window of resolution
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML Works");

	//Define the size of rectangle
	AG::Vector2<float> rectSize = AG::Vector2<float>::one * 100;

	//Define the postion of the rectangle
	AG::Vector2<float> rectPos = AG::Vector2<float>(900, 450);

	//Load the texture
	sf::Texture rectTexture; rectTexture.loadFromFile("Textures/robotronsprites.jpg");


	const int rows = 10;
	const int colums = 21;

	//Number of sprites in the texture 
	AG::Vector2i spritesInSpriteSheet(colums, rows);

	//Returns size of texture (608x512)
	AG::Vector2i rectTextureSize(rectTexture.getSize());



										         // 512/10 * 5 = 256							// 608/21 = 28.9							// 512/10 = 51.2
	//sf::IntRect rectTextureUV(0, (rectTextureSize.y / spritesInSpriteSheet.y) * 5, rectTextureSize.x / spritesInSpriteSheet.x, rectTextureSize.y / spritesInSpriteSheet.y);
	sf::IntRect rectTextureUV(0, 0, 24, 24);
	sf::IntRect altrectTextureUV(35, 0, 24, 24);

	// (0,256 ) are the coordinates on the texture/sprite sheet
	//(28.9, 51.2 ) is the size of how much you want to cookie cutter out of the sprite sheet

	//Create a rectangle
	sf::RectangleShape shape(rectSize);


	//Set texture to be the whole sprite sheet
	shape.setTexture(&rectTexture);

	//Set the texture to the cookie cutter section of the sprite sheet
	shape.setTextureRect(rectTextureUV);

	//Set Rect to the middle
	shape.setOrigin(rectSize / 2);

	//Set the postion of the rect
	shape.setPosition(rectPos);

	shape.setOutlineThickness(3);
	shape.setOutlineColor(sf::Color(255, 255, 255));


	//Get the time before getting the 
	std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
	float deltaTime = 0.0f;
	float timeSincePhysicsStep = 0.0f;


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


			if (event.type == sf::Event::MouseButtonPressed)
			{
				shape.setTextureRect(altrectTextureUV);
			}
			else if (event.type == sf::Event::MouseButtonReleased)
			{
				shape.setTextureRect(rectTextureUV);
			}

		}

		std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
		deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - lastTime).count() / 100000.0f;
		lastTime = now;


		//Clear
		window.clear();

		//Actually add the circle to the screen
		window.draw(shape);

		//Display whats actually been rendered
		window.display();

		
		AG::Event<int, int>eventTest;

		eventTest(5, 6);



		//std::cout << "Delta Time: " << deltaTime << " seconds" << std::endl;
		
		timeSincePhysicsStep += deltaTime;
		while (timeSincePhysicsStep > FIXEDFRAMERATE)
		{

			//std::cout << "timeSincePhysicsStep: " << timeSincePhysicsStep << std::endl;


			timeSincePhysicsStep -= FIXEDFRAMERATE;
		}


		

	}

	return 0;
}


