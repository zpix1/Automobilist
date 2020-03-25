#pragma once

#include <SFML/Graphics.hpp>

#include "utils.h"

struct Segment {
	float scale;
	float curve;
	float spriteX, clip;
	sf::Sprite sprite;
	// X, Y, Z
	sf::Vector3f world;

	// X, Y, W
	sf::Vector3f screen;

	Segment();
	void project(sf::Vector3f& camera);
	void draw_sprite(sf::RenderWindow& app);

};

