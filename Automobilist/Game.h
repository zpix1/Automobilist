#pragma once

#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Segment.h"
#include "utils.h"

class Game {
	sf::RenderWindow* window;
	sf::Font main_font;
	sf::Texture bg;
	sf::Sprite background;
	sf::Sprite player;

	std::vector<sf::Texture> textures;

	// Cooridinates of segment bottom line center
	std::vector<Segment> segments;
	
	float player_x = 0;
	float position = 0;
	float speed = 0;
	float x_speed = 0;
	
	void process_keypress(float dt);
	void render_info();
	void render_player();
public:
	Game(sf::RenderWindow* w);
	void render(sf::Event event);
	void update(float dt);
};

