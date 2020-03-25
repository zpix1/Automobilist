#include "Segment.h"

Segment::Segment() {
	world = sf::Vector3f(0, 0, 0);
	screen = sf::Vector3f(0, 0, 0);

	curve = 0;
	scale = 0;
	spriteX = 0;
	clip = 0;
}

void Segment::project(sf::Vector3f& camera) {
	scale = camera_depth / (world.z - camera.z);
	screen.x = (1 + scale * (world.x - camera.x)) * width / 2;
	screen.y = (1 - scale * (world.y - camera.y)) * height / 2;
	screen.z = scale * road_width * width / 2;
}

void Segment::draw_sprite(sf::RenderWindow& app) {
	sf::Sprite s = sprite;
	int w = s.getTextureRect().width;
	int h = s.getTextureRect().height;

	float destX = screen.x + scale * spriteX * width / 2;
	float destY = screen.y + 4;
	float destW = w * screen.z / 266;
	float destH = h * screen.z / 266;

	destX += destW * spriteX; //offsetX
	destY += destH * (-1);    //offsetY

	float clipH = destY + destH - clip;
	if (clipH < 0) clipH = 0;

	if (clipH >= destH) return;
	s.setTextureRect(sf::IntRect(0, 0, w, h - h * clipH / destH));
	s.setScale(destW / w, destH / h);
	s.setPosition(destX, destY);
	app.draw(s);
}