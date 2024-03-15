#include <SFML/Graphics.hpp>

int main()
{
	//Create window of resolution and app name
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML Works");

	
	//Create a circle shape of radius size 100
	sf::CircleShape shape(100.0f);

	//Fill the circle in green
	shape.setFillColor(sf::Color::Green);

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
				shape.setFillColor(sf::Color::Blue);
			}
			else if (event.type == sf::Event::MouseButtonReleased)
			{
				shape.setFillColor(sf::Color::Green);
			}
		}

		window.clear();

		//Actually add the circle to the screen
		window.draw(shape);

		//Display whats actually been rendered
		window.display();

	}


	return 0;
}
