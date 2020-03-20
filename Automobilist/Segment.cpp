#include "Segment.h"

Segment::Segment() {
	world = sf::Vector3f(0, 0, 0);
	screen = sf::Vector3f(0, 0, 0);

	curve = 0;
	scale = 0;
}

void Segment::project(sf::Vector3f& camera) {
	scale = camera_depth / (world.z - camera.z);
	screen.x = (1 + scale * (world.x - camera.x)) * width / 2;
	screen.y = (1 - scale * (world.y - camera.y)) * height / 2;
	screen.z = scale * road_width * width / 2;
}