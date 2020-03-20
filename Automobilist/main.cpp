#include <SFML/Graphics.hpp>

#include "Game.h"

int main() {
	sf::RenderWindow window(sf::VideoMode(width, height), "Automobilist");
	window.setFramerateLimit(60);
	Game game(&window);

	while (window.isOpen()) {

		window.clear(background);

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		game.render(event);
		window.display();
	}

	return 0;
}