#pragma once

#include <SFML/Graphics.hpp>

#include "utils.h"

struct Segment {
	float scale;

	// X, Y, Z
	sf::Vector3f world_top;
	sf::Vector3f world_bottom;

	// X, Y, W
	sf::Vector3f screen_top;
	sf::Vector3f screen_bottom;

	void project(sf::Vector3f& camera);

};

