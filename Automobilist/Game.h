#pragma once

#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Segment.h"
#include "utils.h"

class Game {
	sf::RenderWindow* window;
	sf::Font main_font;

	// Cooridinates of segment bottom line center
	std::vector<Segment> segments;
	
	float playerX = 0;
	float position = 0;
	float speed = 300;

	void process_keypress();
	void render_info();
public:
	Game(sf::RenderWindow* w);
	void render(sf::Event event);
};

