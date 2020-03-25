#include "Game.h"

#include <string>

Game::Game(sf::RenderWindow* w) {

	if (!main_font.loadFromFile("./Xenogears.ttf")) {
		printf("Can't load font\n");
	}

	window = w;
	if (!bg.loadFromFile("textures/background.png")) {
		printf("Can't load background texture\n");
	}
	bg.setRepeated(true);
	background.setTexture(bg);
	for (int i = 0; i < 1; i++) {
		sf::Texture t;
		textures.push_back(t);
		textures[i].loadFromFile("textures/" + std::to_string(i) + ".png");
	}
	
	
	for (int i = 0; i < segments_buffer_size; i++) {
		Segment segment;

		segment.world.z = i * segment_length;
		segment.curve = 0.3;
		if (i % 100 == 0) {
			segment.spriteX = 1.0;
			segment.sprite.setTexture(textures[0]);
		}
		if (i > 100) {
			segment.world.y = sin((i) / 30.0) * 1500;
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
	sprintf_s(s, "speed: %0.1fkm/s / %f\nposition: %0.1f\nplayerX: %f/%f", speed, max_speed, position, playerX, road_width);
	atext.setString(s);
	window->draw(atext);
}

void Game::process_keypress(float dt) {
	float dx = dt * 2 * (speed / max_speed) * 1600;
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			playerX += std::max(0.0f, dx);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			playerX -= std::max(0.0f, dx);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			speed += accel * dt;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			speed -= accel * dt;
			if (speed > 0) {
				speed += breaking * dt;
			}
			speed = std::max(-max_re_speed, speed);
		}
	}

	if (speed > 0) {
		speed -= decel * dt;
		speed = std::max(speed, 0.0f);
	}

	if (fabs(playerX) > road_width && speed > off_road_limit) {
		speed -= off_road_decel * dt;
	}

	int start_segment_i = ceil(position / segment_length);

	speed = std::min(speed, max_speed);
	position = std::max(0.0f, position + speed);
	playerX = playerX - (dx * segments[start_segment_i].curve * centrifugal);
}

void Game::update(float dt) {
	process_keypress(dt);
}

void Game::render(sf::Event event) {
	window->draw(background);

	render_info();
	int start_segment_i = ceil(position / segment_length);

	float segment_x_offset = position - start_segment_i * segment_length;
	float segement_x_diff = segments[(start_segment_i + 1) % segments_buffer_size].world.z - segments[start_segment_i % segments_buffer_size].world.z;
	float segement_y_diff = segments[(start_segment_i + 1) % segments_buffer_size].world.y - segments[start_segment_i % segments_buffer_size].world.y;
	float segment_y_offset = segment_x_offset * segement_y_diff / segement_x_diff;

	float camera_height = 1500 + segments[(start_segment_i) % segments_buffer_size].world.y + segment_y_offset;
	float x = 0;
	float dx = 0;
	float maxy = height;
	for (int i = start_segment_i; i < start_segment_i + draw_distance; i++) {
		Segment& s = segments[i % segments_buffer_size];
		sf::Vector3f camera(playerX - x, camera_height, position);
		s.project(camera);

		x += dx;
		dx += s.curve;

		s.clip = maxy;
		if (s.screen.y >= maxy) continue;

		maxy = s.screen.y;
		if (i < 1) continue;

		Segment old = segments[(i - 1) % segments_buffer_size];
		// Grass
		draw_quad(*window, sf::Vector3f(0, s.screen.y, width), sf::Vector3f(0, old.screen.y, width), (i / 3) % 2 ? grass1_color : grass2_color);

		// Rumble
		draw_quad(*window, sf::Vector3f(s.screen.x, s.screen.y, s.screen.z * rumble_width_k), sf::Vector3f(old.screen.x, old.screen.y, old.screen.z * rumble_width_k), (i / 3) % 2 ? rumble1_color : rumble2_color);

		// Segment
		draw_quad(*window, s.screen, old.screen, (i / 3) % 2 ? road1_color : road2_color);
	}

	for (int i = start_segment_i + 300; i > start_segment_i; i--) {
		segments[i % segments_buffer_size].draw_sprite(*window);
	}

	window->display();

}
