#include "Game.h"

#include <string>

void Game::load_textures() {
	if (!main_font.loadFromFile("./Xenogears.ttf")) {
		fprintf(stderr, "[ERROR] Can't load font\n");
	}

	if (!background_texture.loadFromFile("textures/background.png")) {
		fprintf(stderr, "[ERROR] Can't load background texture\n");
	}

	background_texture.setRepeated(true);
	background.setTexture(background_texture);

	for (int i = 0; i < total_textures; i++) {
		sf::Texture temp_texture;
		textures.push_back(temp_texture);
		if (!textures[i].loadFromFile("textures/" + std::to_string(i) + ".png")) {
			printf("[ERROR] Can't load texture textures/%d.png", i);
		}
	}

	player.sprite.setTexture(textures[1]);
}

void Game::fill_segments() {
	for (int i = 0; i < segments_buffer_size; i++) {
		Segment segment;

		segment.world.z = i * segment_length;

		// Add hills
		if (i > 0) {
			segment.world.y = sin((i) / 30.0) * 1500;
		}
		// Add curves
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
		// Add sprites
		if (i % 10 == 0) {
			segment.sprite_x = 3.0;
			segment.sprite.setTexture(textures[2]);
		}

		segments.push_back(segment);
	}
}

void Game::reset_cars() {
	// Remove old cars
	cars.clear();

	for (int i = 0; i < total_cars; i++) {
		Car car;
		car.x = randint(-1, 1) * 2.0 / 3.0;
		car.position = 1000 * i * segment_length;
		car.speed =  (30.0 + 0 * ((float)rand() / RAND_MAX));
		car.sprite.setTexture(textures[1]);

		std::shared_ptr<Car> ptr = std::make_shared<Car>(car);

		cars.push_back(ptr);
		segments[find_segment_i(car.position)].cars.push_back(ptr);
	}
}

Game::Game(sf::RenderWindow* w) {
	window = w;
	load_textures();
	fill_segments();
	reset_cars();
}

void Game::render_info() {
	sf::Text info_panel;
	info_panel.setCharacterSize(20);
	info_panel.setFillColor(sf::Color(0, 0, 0));
	info_panel.setPosition(0, 0);
	info_panel.setFont(main_font);
	char info_text[200];
	sprintf_s(info_text, "speed: %0.1fkm/s / %f\nposition: %0.1f\npx: %f", camera_speed, max_speed, camera_position, camera_x);
	info_panel.setString(info_text);
	window->draw(info_panel);
}

void Game::process_keypress(float dt) {
	camera_x_speed = 2.0 * (camera_speed / max_speed) * max_x_speed;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		camera_x += std::max(0.0f, camera_x_speed * dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		camera_x -= std::max(0.0f, camera_x_speed * dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		camera_speed += acceleration_value * dt;
	}
	else {
		if (camera_speed > 0) {
			camera_speed -= deceleration_value * dt;
			camera_speed = std::max(camera_speed, 0.0f);
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		camera_speed -= acceleration_value * dt;
		if (camera_speed > 0) {
			camera_speed -= breaking_value * dt;
		}
		camera_speed = std::max(-reverse_max_speed, camera_speed);
	}


	if (fabs(camera_x) > road_width && camera_speed > off_road_max_speed) {
		camera_speed -= off_road_deceleration_value * dt;
	}


	camera_speed = std::min(camera_speed, max_speed);
	camera_position = std::max(0.0f, camera_position + camera_speed);

	while (camera_position >= segments_buffer_size * segment_length) camera_position -= segments_buffer_size * segment_length;
	while (camera_position < 0) camera_position += segments_buffer_size * segment_length;

	int start_segment_i = find_segment_i(camera_position);
	camera_x_speed = (camera_x_speed * dt * segments[start_segment_i].curve * centrifugal_k / 3.0);
	camera_x = camera_x - camera_x_speed;
}

void Game::update_cars(float dt) {
	for (auto c : cars) {
		Segment& s1 = segments[find_segment_i(c->position)];
		auto to_delete = std::find(s1.cars.begin(), s1.cars.end(), c);
		s1.cars.erase(to_delete);
		c->position += c->speed;
		Segment& s2 = segments[find_segment_i(c->position)];
		s2.cars.push_back(c);
	}
}

void Game::process_collisions() {
	Segment& s = segments[find_segment_i(camera_position)];

	/*for (auto c : s.cars) {
		if (speed >= c->speed)
		if (overlap(player_x / road_width, 1, c->x, 1, 0.1)) {
			printf("OVERLAP speed = %f car_speed = %f px=%f cx=%f\n", speed, c->speed, player_x / width, c->x);
			
			speed = 25;
		}
	}*/
}

void Game::update(float dt) {
	process_keypress(dt);
	update_cars(dt);
}

void Game::render_player() {
	player.x = camera_x / road_width;
	player.position = camera_position + segment_length * 6;

	int start_segment_i = find_segment_i(player.position);

	Segment s1 = segments[start_segment_i];
	Segment s2 = segments[(start_segment_i + 1) % segments_buffer_size];

	sf::Sprite s = player.sprite;
	int w = s.getTextureRect().width;
	int h = s.getTextureRect().height;

	float percent = get_segment_percent(player.position);
	float destX = interpolate(s1.screen.x, s2.screen.x, percent);// + interpolate(s1.scale, s2.scale, percent) * road_width * c.x / 2;
	float destY = interpolate(s1.screen.y, s2.screen.y, percent);
	float destW = w * interpolate(s1.scale, s2.scale, percent) * scale_to_car_k * player_scale;
	float destH = h * interpolate(s1.scale, s2.scale, percent) * scale_to_car_k * player_scale;

	destX += interpolate(s1.scale, s2.scale, percent) * player.x * road_width * width / player_scale;
	destY -= destH;

	if (camera_x_speed < 0) {
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

	s.setScale(destW / w, destH / h);
	s.setPosition(destX - destW / 2, destY);
	window->draw(s);
}

void Game::render(sf::Event event) {
	process_collisions();

	window->draw(background);

	render_info();

	int start_segment_i = (int)ceil(camera_position / segment_length) % segments_buffer_size;

	float segment_x_offset = camera_position - start_segment_i * segment_length;
	float segement_x_diff = segments[(start_segment_i + 1) % segments_buffer_size].world.z - segments[start_segment_i].world.z;
	float segement_y_diff = segments[(start_segment_i + 1) % segments_buffer_size].world.y - segments[start_segment_i].world.y;
	float segment_y_offset = segment_x_offset * segement_y_diff / segement_x_diff;

	float camera_height = camera_height_offset + segments[start_segment_i].world.y + segment_y_offset;
	float x = 0;
	float dx = 0;
	float max_y = height;

	float segment_percent = get_segment_percent(camera_position);

	dx = -segments[start_segment_i].curve * segment_percent;

	for (int i = start_segment_i; i < start_segment_i + draw_distance; i++) {
		Segment& s1 = segments[i % segments_buffer_size];
		
		sf::Vector3f camera(camera_x - x, camera_height, camera_position - (i >= segments_buffer_size ? segments_buffer_size * segment_length : 0));
		
		s1.project(camera);

		x += dx;
		dx += s1.curve;

		s1.clip = max_y;
		if (s1.screen.y >= max_y) continue;
		max_y = s1.screen.y;

		if (i < 1) continue;

		Segment s2 = segments[(i - 1) % segments_buffer_size];
		
		// Grass
		draw_quad(*window, sf::Vector3f(0, s1.screen.y, width), sf::Vector3f(0, s2.screen.y, width), (i / 3) % 2 ? grass1_color : grass2_color);

		// Rumble
		draw_quad(*window, sf::Vector3f(s1.screen.x, s1.screen.y, s1.screen.z * rumble_width_k), sf::Vector3f(s2.screen.x, s2.screen.y, s2.screen.z * rumble_width_k), (i / 3) % 2 ? rumble1_color : rumble2_color);

		// Segment
		draw_quad(*window, s1.screen, s2.screen, (i / 3) % 2 ? road1_color : road2_color);
	}

	for (int i = start_segment_i + draw_distance; i > start_segment_i; i--) {
		segments[i % segments_buffer_size].draw_sprite(*window);

		Segment s1 = segments[i % segments_buffer_size];
		if ((i + 1) % segments_buffer_size == 0 || (i + 1) == (start_segment_i + draw_distance)) continue;
		Segment s2 = segments[(i + 1) % segments_buffer_size];


		for (int car_i = 0; car_i < s1.cars.size(); car_i++) {
			Car& c = *s1.cars[car_i];
			sf::Sprite s = c.sprite;
			int w = s.getTextureRect().width;
			int h = s.getTextureRect().height;

			float percent = get_segment_percent(c.position);
			float destX = interpolate(s1.screen.x, s2.screen.x, percent);// + interpolate(s1.scale, s2.scale, percent) * road_width * c.x / 2;
			float destY = interpolate(s1.screen.y, s2.screen.y, percent);
			float destW = w * interpolate(s1.scale, s2.scale, percent) * scale_to_car_k * car_scale;
			float destH = h * interpolate(s1.scale, s2.scale, percent) * scale_to_car_k * car_scale;

			destX += c.x * interpolate(s1.scale, s2.scale, percent) * road_width * width / car_scale;
			destY -= destH;

			float clip_height = destY + destH - s2.clip;

			if (clip_height < 0) clip_height = 0;
			if (clip_height >= destH) continue;

			s.setTextureRect(sf::IntRect(0, 0, w, h - h * clip_height / destH));
			s.setScale(destW / w, destH / h);
			s.setPosition(destX - destW / 2, destY);
			window->draw(s);
		}

	}

	render_player();

	window->display();
}
