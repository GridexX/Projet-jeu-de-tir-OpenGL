#include <cstdlib>
#include <iostream>
#include <string>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "demoSFML/myapp.hpp"

int main(int argc, char* argv[]) {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML", sf::Style::Default, settings);
	MyApp app(window);
	window.clear(sf::Color::White);
	app.update();
	window.display();

	while (window.isOpen())
	{
		/*sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::Resized)
			{
				sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
				window.setView(sf::View(visibleArea));
			}
		}

		window.clear(sf::Color::White);
		app.update();

		window.display();*/

		sf::Event event;
		if (window.waitEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::Resized) {
				sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
				window.setView(sf::View(visibleArea));
				window.clear(sf::Color::White);
				app.update();
				window.display();
			}
		}
	}
		
	return EXIT_SUCCESS;
}
