#include "Segment.h"

Segment::Segment() {
	world_top = sf::Vector3f(0, 0, 0);
	world_bottom = sf::Vector3f(0, 0, 0);
	screen_top = sf::Vector3f(0, 0, 0);
	screen_bottom = sf::Vector3f(0, 0, 0);

	curve = 0;
	scale = 0;
}

void Segment::project(sf::Vector3f& camera) {
	scale = camera_depth / (world_top.z - camera.z);
	screen_top.x = (1 + scale * (world_top.x - camera.x)) * width / 2;
	screen_top.y = (1 - scale * (world_top.y - camera.y)) * height / 2;
	screen_top.z = scale * road_width * width / 2;

	scale = camera_depth / (world_bottom.z - camera.z);
	screen_bottom.x = (1 + scale * (world_bottom.x - camera.x)) * width / 2;
	screen_bottom.y = (1 - scale * (world_bottom.y - camera.y)) * height / 2;
	screen_bottom.z = scale * road_width * width / 2;
}