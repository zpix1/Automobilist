#include "Game.h"

#include <string>

Game::Game(sf::RenderWindow* w) {

	if (!main_font.loadFromFile("./Xenogears.ttf")) {
		printf("BAD\n");
	}

	window = w;

	for (int i = 0; i < segments_buffer_size; i++) {
		Segment segment;

		segment.world_top.z = (i + 1) * segment_length;
		segment.world_bottom.z = i * segment_length;

		//segment.curve = (i > 300 && i < 700) ? 0.5 : 0;
		if (i > 100) {
			segment.world_top.y = sin((i + 1) / 30.0) * 1500;
			segment.world_bottom.y = sin(i / 30.0) * 1500;
		}

		segments.push_back(segment);
	}

}

void Game::render_info() {
	sf::Text atext;
	atext.setCharacterSize(20);
	atext.setFillColor(sf::Color(0, 0, 0));
	atext.setPosition(0, 0);
	atext.setFont(main_font);
	char s[200];
	sprintf_s(s, "speed: %0.1fkm/s\nposition: %0.1f", speed, position);
	atext.setString(s);

	//draw the string
	window->draw(atext);
}

void Game::process_keypress() {
	if (speed > 0) {
		speed = std::max(0.0f, speed - decel);
	}
	else {
		speed = std::min(0.0f, speed + decel);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		playerX -= x_speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		playerX += x_speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		speed += accel;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		speed -= accel;
	}
	speed = std::min(speed, max_speed);
	position = std::max(0.0f, position + speed);
}

void Game::render(sf::Event event) {
	process_keypress();
	render_info();
	int start_segment_i = position / segment_length;
	float camera_height = 1500 + segments[(start_segment_i) % segments_buffer_size].world_bottom.y;
	float x = 0;
	float dx = 0;
	int maxy = height;
	for (int i = start_segment_i; i < start_segment_i + draw_distance; i++) {
		Segment& s = segments[i % segments_buffer_size];
		sf::Vector3f camera(playerX - x, camera_height, position);
		s.project(camera);

		x += dx;
		dx += s.curve;

		if (s.screen_bottom.y >= maxy) continue;
		maxy = s.screen_bottom.y;

		// Grass
		draw_quad(*window, sf::Vector3f(0, s.screen_top.y, width), sf::Vector3f(0, s.screen_bottom.y, width), (i / 3) % 2 ? grass1_color : grass2_color);

		// Rumble
		draw_quad(*window, sf::Vector3f(s.screen_top.x, s.screen_top.y, s.screen_top.z * rumble_width_k), sf::Vector3f(s.screen_bottom.x, s.screen_bottom.y, s.screen_bottom.z * rumble_width_k), (i / 3) % 2 ? rumble1_color : rumble2_color);

		// Segment
		draw_quad(*window, s.screen_top, s.screen_bottom, (i / 3) % 2 ? road1_color : road2_color);
	}
}
