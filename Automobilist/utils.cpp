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