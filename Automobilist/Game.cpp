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
	for (int i = 0; i < amount_of_textures; i++) {
		sf::Texture t;
		textures.push_back(t);
		textures[i].loadFromFile("textures/" + std::to_string(i) + ".png");
	}
	player.setTexture(textures[1]);


	for (int i = 0; i < segments_buffer_size; i++) {
		Segment segment;

		segment.world.z = i * segment_length;
		switch ((i / 300) % 3) {
		case 0:
			segment.curve = 3.0;
			break;
		case 1:
			segment.curve = 3.0;
			break;
		case 2:
			segment.curve = -3.7;
			break;
		}
		if (i % 100 == 0) {

			segment.spriteX = 2.0;
			if (i / 100 % 2 == 0) {
				segment.spriteX = -2.0;
			}
			segment.sprite.setTexture(textures[2]);
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
	sprintf_s(s, "speed: %0.1fkm/s / %f\nposition: %0.1f\npx: %f", speed, max_speed, position, player_x);
	atext.setString(s);
	window->draw(atext);
}

void Game::process_keypress(float dt) {
	x_speed = 2.0 * (speed / max_speed) * max_x_speed;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		player_x += std::max(0.0f, x_speed * dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		player_x -= std::max(0.0f, x_speed * dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		speed += accel * dt;
	}
	else {
		if (speed > 0) {
			speed -= decel * dt;
			speed = std::max(speed, 0.0f);
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		speed -= accel * dt;
		if (speed > 0) {
			speed -= breaking * dt;
		}
		speed = std::max(-max_reverse_speed, speed);
	}


	if (fabs(player_x) > road_width && speed > off_road_limit) {
		speed -= off_road_decel * dt;
	}


	speed = std::min(speed, max_speed);
	position = std::max(0.0f, position + speed);

	while (position >= segments_buffer_size * segment_length) position -= segments_buffer_size * segment_length;
	while (position < 0) position += segments_buffer_size * segment_length;

	int start_segment_i = floor(position / segment_length);
	x_speed = (x_speed * dt * segments[start_segment_i].curve * centrifugal / 3.0);
	player_x = player_x - x_speed;
}

void Game::update(float dt) {
	process_keypress(dt);
}

void Game::render_player() {
	sf::Sprite s = player;
	int w = s.getTextureRect().width;
	int h = s.getTextureRect().height;

	if (x_speed < 0) {
		s.setTextureRect(sf::IntRect(w, 0, -w, h));
	}
	else {
		s.setTextureRect(sf::IntRect(0, 0, w, h));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		s.setTextureRect(sf::IntRect(0, 0, w, h));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		s.setTextureRect(sf::IntRect(w, 0, -w, h));
	}
	s.setScale(2, 2);
	s.setPosition(width / 2 - w / 2, height - h * 2 - 10);
	window->draw(s);
}

void Game::render(sf::Event event) {
	window->draw(background);

	render_info();

	int start_segment_i = ceil(position / segment_length);

	float segment_x_offset = position - start_segment_i * segment_length;
	float segement_x_diff = segments[(start_segment_i + 1) % segments_buffer_size].world.z - segments[start_segment_i % segments_buffer_size].world.z;
	float segement_y_diff = segments[(start_segment_i + 1) % segments_buffer_size].world.y - segments[start_segment_i % segments_buffer_size].world.y;
	float segment_y_offset = segment_x_offset * segement_y_diff / segement_x_diff;

	float camera_height = s_camera_height + segments[(start_segment_i) % segments_buffer_size].world.y + segment_y_offset;
	float x = 0;
	float dx = 0;
	float maxy = height;

	for (int i = start_segment_i; i < start_segment_i + draw_distance; i++) {
		Segment& s = segments[i % segments_buffer_size];
		sf::Vector3f camera(player_x - x, camera_height, position - (i >= segments_buffer_size ? segments_buffer_size * segment_length : 0));
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

	render_player();

	window->display();

}
