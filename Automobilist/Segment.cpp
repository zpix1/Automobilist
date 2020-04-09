#include "Segment.h"

Segment::Segment() {
	world = sf::Vector3f(0, 0, 0);
	screen = sf::Vector3f(0, 0, 0);

	curve = 0;
	scale = 0;
	sprite_x = 0;
	clip = 0;
}

void Segment::project(sf::Vector3f& camera) {
	scale = camera_depth / (world.z - camera.z);
	screen.x = (1.0 + scale * (world.x - camera.x)) * width / 2.0;
	screen.y = (1.0 - scale * (world.y - camera.y)) * height / 2.0;
	screen.z = scale * road_width * width / 2.0;
}

void Segment::draw_sprite(sf::RenderWindow& app) {
	sf::Sprite decoration_sprite = sprite;
	int w = decoration_sprite.getTextureRect().width;
	int h = decoration_sprite.getTextureRect().height;

	float destination_x = screen.x + scale * sprite_x * road_width / 2;
	float destination_y = screen.y;
	float destination_w = w * screen.z / 266;
	float destination_h = h * screen.z / 266;

	destination_x += destination_w * sprite_x;
	destination_y -= destination_h;

	float clip_height = destination_y + destination_h - clip;
	if (clip_height < 0) clip_height = 0;

	if (clip_height >= destination_h) return;
	decoration_sprite.setTextureRect(sf::IntRect(0, 0, w, h - h * clip_height / destination_h));
	decoration_sprite.setScale(destination_w / w, destination_h / h);
	decoration_sprite.setPosition(destination_x, destination_y);  
	app.draw(decoration_sprite);
}