#pragma once

#include <SFML/Graphics.hpp>

#include "utils.h"

struct Segment {
	float scale;
	float curve;

	// X, Y, Z
	sf::Vector3f world;

	// X, Y, W
	sf::Vector3f screen;

	Segment();
	void project(sf::Vector3f& camera);

};

