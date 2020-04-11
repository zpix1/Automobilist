#include <SFML/Graphics.hpp>

#include "Game.h"

int main() {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	
	sf::RenderWindow window(sf::VideoMode(width, height), "Automobilist", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);

	Game game(&window);
	sf::Clock clock;
	float last = clock.getElapsedTime().asMilliseconds();
	float gdt = 0;

#if (!DEBUG)
	srand(time(NULL));
#endif

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		float now = clock.getElapsedTime().asMilliseconds();
		float dt = std::min(1.f, (now - last) / 1000.0f);
		gdt = gdt + dt;

		while (gdt > step) {
			gdt = gdt - step;
			//printf("fps: %f\r", 1.0f / gdt);
			game.update(step);
		}
		
		last = now;

		game.render(event);
	}

	return 0;
}