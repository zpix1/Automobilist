#include <SFML/Graphics.hpp>

#include "Game.h"

int main() {
	sf::RenderWindow window(sf::VideoMode(width, height), "Automobilist");

	Game game(&window);

	while (window.isOpen()) {

		sf::Event event;
		while (window.pollEvent(event)) {
			window.clear(sf::Color::White);
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			game.render(event);
			window.display();
		}

	}

	return 0;
}