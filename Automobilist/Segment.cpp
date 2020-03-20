#include "Segment.h"

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