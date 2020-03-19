#include "Game.h"


Game::Game(sf::RenderWindow* w) {
	window = w;

	for (int i = 0; i < segments_buffer_size; i++) {
		Segment segment;
		// TODO
		segments.push_back(segment);
	}

}

void Game::process_keypress() {
	position += speed;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		playerX -= 20;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		playerX += 20;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		speed += accel;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		speed -= accel;
	}
}

void Game::render(sf::Event event) {
	for (int i = 0; i < draw_distance; i++) {
		
	}
}
