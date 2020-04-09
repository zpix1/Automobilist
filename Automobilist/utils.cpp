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