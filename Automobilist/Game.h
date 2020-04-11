#pragma once

#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Segment.h"
#include "utils.h"

class Game {
	sf::RenderWindow* window;
	sf::Font main_font;

	sf::Sprite background;

	Car player;
	std::vector< std::shared_ptr<Car> > cars;

	sf::Texture background_texture;
	std::vector<sf::Texture> textures;

	std::vector<Segment> segments;

	float camera_x = 0;
	float camera_position = 0;
	float camera_speed = 0;
	float camera_x_speed = 0;
	bool dont_change_pos = false;
	

	void load_textures();
	void fill_segments();
	void reset_cars();

	void process_keypress(float dt);
	void update_cars(float dt);
	void process_collisions();

	void render_info();
	void render_player();

public:
	Game(sf::RenderWindow* w);
	void render(sf::Event event);
	void update(float dt);
};

