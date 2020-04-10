#include "utils.h"

void draw_quad(sf::RenderWindow& window, sf::Vector3f screen_top, sf::Vector3f screen_bottom, sf::Color color) {
	sf::ConvexShape shape(4);
	shape.setFillColor(color);
	shape.setPoint(0, sf::Vector2f(screen_top.x - screen_top.z, screen_top.y));
	shape.setPoint(1, sf::Vector2f(screen_top.x + screen_top.z, screen_top.y));
	shape.setPoint(2, sf::Vector2f(screen_bottom.x + screen_bottom.z, screen_bottom.y));
	shape.setPoint(3, sf::Vector2f(screen_bottom.x - screen_bottom.z, screen_bottom.y));
	window.draw(shape);
}

void draw_sprite(sf::RenderWindow& window, sf::Sprite sprite, Segment& s1, Segment& s2, float position, float x, float scale, bool mirrored) {
	int w = sprite.getTextureRect().width;
	int h = sprite.getTextureRect().height;

	float percent = get_segment_percent(position);
	float destination_x = interpolate(s1.screen.x, s2.screen.x, percent);// + interpolate(s1.scale, s2.scale, percent) * road_width * c.x / 2;
	float destination_y = interpolate(s1.screen.y, s2.screen.y, percent);
	float destination_w = w * interpolate(s1.scale, s2.scale, percent) * scale_to_car_k * scale;
	float destination_h = h * interpolate(s1.scale, s2.scale, percent) * scale_to_car_k * scale;

	destination_x += x * interpolate(s1.scale, s2.scale, percent) * road_width * width / scale;
	destination_y -= destination_h;

	float clip_height = destination_y + destination_h - s2.clip;

	if (clip_height < 0) clip_height = 0;
	if (clip_height >= destination_h) return;

	if (mirrored) {
		sprite.setTextureRect(sf::IntRect(w, 0, -w, h - h * clip_height / destination_h));
	}
	else {
		sprite.setTextureRect(sf::IntRect(0, 0, w, h - h * clip_height / destination_h));
	}

	sprite.setScale(destination_w / w, destination_h / h);
	sprite.setPosition(destination_x - destination_w / 2, destination_y);
	window.draw(sprite);
}

float interpolate(float a, float b, float percent) {
	return a + (b - a) * percent;
}

bool overlap(float x1, float w1, float x2, float w2, float percent) {
	float half = (percent) / 2;
	float min1 = x1 - (w1 * half);
	float max1 = x1 + (w1 * half);
	float min2 = x2 - (w2 * half);
	float max2 = x2 + (w2 * half);
	return !((max1 < min2) || (min1 > max2));
}

int randint(int min, int max) {
	return min + (rand() % static_cast<int>(max - min + 1));
}

int find_segment_i(float position) {
	return (int)floor(position / segment_length) % segments_buffer_size;
}

float get_segment_percent(float position) {
	int i = (int)floor(position / segment_length);
	return (position - i * segment_length) / segment_length;
}

float get_lane_x(int lane_i) {
	lane_i %= total_lanes;
	return 2.0 / total_lanes * lane_i + 1.0 / total_lanes - 1.0;
}