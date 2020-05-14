#include "Game.h"

#include <string>
#include <algorithm>

void Game::render_start_info() {
    window->clear(sf::Color::Black);
    sf::Text info_panel;
    info_panel.setCharacterSize(40);
    info_panel.setFillColor(sf::Color::White);
    info_panel.setPosition(100, 100);
    info_panel.setFont(main_font);
    char info_text[] = \
        "Welcome to Automobilist!\n"
        "It is your last chance to pass a driving exam!\n"
        "If you fail it this time\nyou will never get a driver license again...\n"
        "Pull yourself together, you hear me??\n\nUse arrow keys to control the car.\n"
        "You have to get it done in 5 minutes\n  and with no more than 5 errors (stars).\n\n"
        "Press space to continue,\n good luck!";
    info_panel.setString(info_text);
    window->draw(info_panel);
    window->display();
}

void Game::render_won_info() {
    window->clear(sf::Color::Black);
    sf::Text info_panel;
    info_panel.setCharacterSize(40);
    info_panel.setFillColor(sf::Color::White);
    info_panel.setPosition(window_width / 2 - 100, window_height / 2 - 50);
    info_panel.setFont(main_font);
    char info_text[] = \
        "You won!\n Well done!";
    info_panel.setString(info_text);
    window->draw(info_panel);
    window->display();
}

void Game::render_lost_info() {
    window->clear(sf::Color::Black);
    sf::Text info_panel;
    info_panel.setCharacterSize(40);
    info_panel.setFillColor(sf::Color::White);
    info_panel.setPosition(window_width / 2 - 100, window_height / 2 - 50);
    info_panel.setFont(main_font);
    char info_text[] = \
        "You lost!\n As usual...";
    info_panel.setString(info_text);
    window->draw(info_panel);
    window->display();
}

void Game::load_textures() {
    if (!main_font.loadFromFile("./assets/Xenogears.ttf")) {
        fprintf(stderr, "[ERROR] Can't load font\n");
    }
    
    std::vector<std::string> tnames = { 
        "bluecar", "palm", "20sign", "60sign", "70sign", "120sign", "xsign", "sky",
        "0stars", "1stars", "2stars", "3stars", "4stars", "5stars",
        "1car", "2car", "3car", "4car",
        "wall", "cactus", "palm_tree"
    };
    for (int i = 0; i < tnames.size(); i++) {
        TextureT temp_texture;
        
        if (!temp_texture.texture.loadFromFile("./assets/" + tnames[i] + ".png")) {
            fprintf(stderr, "[ERROR] Can't load texture textures: %s.png", tnames[i].c_str());
        }

        temp_texture.name = tnames[i];
        textures.push_back(temp_texture);
    }

    background_texture.setRepeated(true);
    background.setTexture(get_texture("sky"));
    player.sprite.setTexture(get_texture("bluecar"));
}

sf::Texture& Game::get_texture(std::string name) {
    for (TextureT& t : textures) {
        if (t.name == name) {
            return t.texture;
        }
    }
}

void Game::fill_segments() {
    const int length_k = 3;
    //              length  length_p speed_l    sign          curve     height  failed (false)  tm          tf
    map.push_back({ 50,     50,      INFINITY,  "xsign",      4.5,      100,    false,           "",        11  });
    map.push_back({ 300,    350,     60,        "60sign",     0.5,      200,    false,          "palm",     15  });
    map.push_back({ 600,    950,     INFINITY,  "xsign",      0.7,      0,      false,          "cactus",   31  });
    map.push_back({ 300,    1250,    60,        "60sign",     2.0,      200,    false,          "palm_tree",21  });
    map.push_back({ 600,    1850,    120,       "120sign",   -1,        150,    false,           "",        10  });
    map.push_back({ 200,    2050,    20,        "20sign",    -4,        0,      false,          "wall",     3   });
    
    std::vector< std::pair<int, float> > heights;
    
    heights.push_back({ 0, 60.f });
    heights.push_back({ 250, 30.f });
    heights.push_back({ 400, 40.f });
    heights.push_back({ 500, 25.f });
    heights.push_back({ 600, 10.f });
    heights.push_back({ 750, 30.f });
    heights.push_back({ 850, 10.f });
    heights.push_back({ 1000, 0.f });
    heights.push_back({ 1100, 25.f });
    heights.push_back({ 1200, 5.f });
    heights.push_back({ 1300, 0.f });
    heights.push_back({ 1400, 60.f });
    heights.push_back({ 1600, 20.f });
    heights.push_back({ 1800, 60.f });
    heights.push_back({ 1900, 40.f });
    heights.push_back({ 2000, 60.f });
    heights.push_back({ 2050, 60.f });

    for (int i = 0; i < heights.size(); i++) {
        heights[i].first *= length_k;
    }

    // Recalculate prefixes
    int acc = 0;
    for (int i = 0; i < map.size(); i++) {
        map[i].length *= length_k;
        acc += map[i].length;
        map[i].length_prefix = acc;
    }

    int global_i = 0;

    int hmi = 0;
    int hmx;
    int hmdx;
    float hmy;
    float hmdy;

    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[i].length; j++, global_i++) {
            if (heights[hmi].first < global_i || global_i == 0) {
                hmx = heights[hmi].first;
                hmy = heights[hmi].second;
                hmi++;
                hmdx = heights[hmi].first - heights[hmi - 1].first;
                hmdy = heights[hmi].second - heights[hmi - 1].second;
            }

            Segment segment;

            segment.world.z = global_i * segment_length;
            segment.speed_limit = map[i].speed_limit / speed_to_screen;
            segment.curve = map[i].curve;

            float t = (global_i - hmx) / (float)hmdx;

            segment.world.y = hmy + hmdy * pbend(t);

            segment.world.y *= 800;
            
            if (j == 0) {
                segment.sprite_x = 1.5;
                segment.sprite.setTexture(get_texture(map[i].sign_name));
            }
            else if (map[i].texture_name.size() != 0 && global_i % map[i].texture_freq == 0) {
                if (global_i % 2 == 0) {
                    segment.sprite_x = -1.5;
                    segment.sprite.setTexture(get_texture(map[i].texture_name));
                }
                else {
                    segment.sprite_x = 1.5;
                    segment.sprite.setTexture(get_texture(map[i].texture_name));
                }
            }
            segments.push_back(segment);
        }
    }
}

void Game::reset_cars() {
    // Remove old cars
    cars.clear();

    for (int i = 0; i < total_cars; i++) {
        Car car;
        int lane_id = randint(0, total_lanes - 1);
        
        car.lane_id = lane_id;
        car.x = get_lane_x(lane_id) + 0.1 * ((((float)rand() / RAND_MAX)) - 0.5);
        car.position = 30 * i * segment_length;
        car.speed =  (max_car_speed - 150.0 * ((float)rand() / RAND_MAX));
        car.max_speed = car.speed;

        int sprite_id = randint(1, 4);

        if (sprite_id <= 2) {
            car.max_speed += 50;
        }

        car.sprite.setTexture(get_texture(std::to_string(sprite_id) + "car"));

        std::shared_ptr<Car> ptr = std::make_shared<Car>(car);

        cars.push_back(ptr);
        segments[find_segment_i(car.position)].cars.push_back(ptr);
    }
}

Game::Game(sf::RenderWindow* w, int* r_ptr) {
    result_ptr = r_ptr;
    window = w;
    timer.restart();
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
    sprintf_s(info_text, "%.1f\nspeed: %0.0fkm/s\n%d:%.2d\n%d/%d", fps, camera_speed * speed_to_screen, (int)timer.getElapsedTime().asSeconds() / 60, (int)timer.getElapsedTime().asSeconds() % 60, lap_count + 1, laps_to_win);
    info_panel.setString(info_text);
    window->draw(info_panel);
}

void Game::add_to_log(std::string str) {
    log.push_back(str);

    if (log.size() > 10) {
        log.erase(log.begin());
    }
}

void Game::render_log() {
    sf::Text info_panel;
    info_panel.setCharacterSize(20);
    info_panel.setFillColor(sf::Color(0, 0, 0));
    info_panel.setPosition(window_width - 310, 70);
    info_panel.setFont(main_font);
    std::string info_text;
    for (int i = 0; i < log.size(); i++) {
        info_text += log[i] + "\n";
    }
    info_panel.setString(info_text);
    window->draw(info_panel);
}

void Game::render_stars() {
    sf::Sprite stars;
    stars.setTexture(get_texture(std::to_string(stars_count) + "stars"));
    stars.setPosition(window_width - 310, 0);
    stars.setScale(0.2, 0.2);
    window->draw(stars);
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
    if (!dont_change_pos)
    camera_position = std::max(0.0f, camera_position + camera_speed);

    while (camera_position >= segments_buffer_size * segment_length) camera_position -= segments_buffer_size * segment_length;
    while (camera_position < 0) camera_position += segments_buffer_size * segment_length;

    int start_segment_i = find_segment_i(camera_position);
    camera_x_speed = (camera_x_speed * dt * segments[start_segment_i].curve * centrifugal_k / 3.0);
    if (!dont_change_pos)
    camera_x = camera_x - camera_x_speed;
}

void Game::update_cars(float dt) {
    for (auto c : cars) {
        Segment* car_segment_ptr = nullptr;
        float s1_speed_limit, s2_speed_limit;
        if (find_segment_i(c->position) != find_segment_i(c->position + c->speed)) {
            Segment& s1 = segments[find_segment_i(c->position)];
            s1_speed_limit = s1.speed_limit;
            auto to_delete = std::find(s1.cars.begin(), s1.cars.end(), c);
            s1.cars.erase(to_delete);

            /*for (auto near_car : s1.cars) {
                if (near_car != c && near_car->speed < c->speed && overlap(c->x * s1.screen.z, c->sprite.getTextureRect().width, near_car->x * s1.screen.z, near_car->sprite.getTextureRect().width, 1)) {
                    c->speed = 25;
                }
            }*/

            c->position += c->speed;

            while (c->position >= segments_buffer_size * segment_length) c->position -= segments_buffer_size * segment_length;
            while (c->position < 0) c->position += segments_buffer_size * segment_length;

            Segment& s2 = segments[find_segment_i(c->position)];
            s2_speed_limit = s2.speed_limit;

            s2.cars.push_back(c);
            car_segment_ptr = &s2;
        }
        else {
            c->position += c->speed;
            Segment& s2 = segments[find_segment_i(c->position)];
            s1_speed_limit = s2_speed_limit = s2.speed_limit;
            car_segment_ptr = &s2;
        }

        Segment& s2 = *car_segment_ptr;
        
        // To avoid collisions
        for (auto near_car : s2.cars) {
            if (near_car != c && near_car->speed < c->speed && c->position <= near_car->position && overlap(c->x * s2.screen.z, c->sprite.getTextureRect().width, near_car->x * s2.screen.z, near_car->sprite.getTextureRect().width, 1)) {
                bool found = false;
                if (1) {
                    for (int lane_id = 0; lane_id < total_lanes; lane_id++) {
                        if (abs(lane_id - c->lane_id) == 1)
                        if (!overlap(get_lane_x(lane_id) * s2.screen.z, c->sprite.getTextureRect().width, near_car->x * s2.screen.z, near_car->sprite.getTextureRect().width, 1)) {
                            c->x = get_lane_x(lane_id);
                            found = true;
                            break;
                        }
                    }
                }
                if (!found) {
                    c->speed = near_car->speed * 0.8;
                }
            }
        }

        // To respect speed limits
        if (s1_speed_limit != s2_speed_limit) {
            if (s2_speed_limit != INFINITY) {
                c->speed = s2_speed_limit - ((float)rand() / RAND_MAX) * 0.2 * s2_speed_limit;
            }
            else {
                c->speed = c->max_speed;
            }
        }

        // To be fast
        if (c->speed < s2.speed_limit) {
            c->speed = std::min(c->speed + 1.0f, c->max_speed);
        }
    }
}

void Game::add_star() {
    if (stars_count == 4) {
        add_to_log("Bamboozle! I give you a chance");
    }
    if (stars_count == 5) {
        *result_ptr = 1;
    }
    stars_count = std::min(stars_count + 1, 5);
}

void Game::process_collisions() {
    Segment& s = segments[find_segment_i(player.position)];

    float player_w = player.sprite.getTextureRect().width * s.scale * scale_to_car_k * player_scale;

    for (auto c : s.cars) {
        if (camera_speed >= c->speed) {
            float car_w = c->sprite.getTextureRect().width * s.scale * scale_to_car_k * car_scale;

            if (overlap(player.x * s.screen.z, player_w, c->x * s.screen.z, car_w, 1)) {
                player.speed = camera_speed = 25;
                add_to_log("You broke a car (+1)");
                add_star();
            }
        }
    }
    {
        float sprite_w = s.sprite.getTextureRect().width * s.scale * scale_to_car_k * car_scale;
        if (sprite_w)
        if (overlap(player.x * s.screen.z, player_w, s.sprite_x * s.screen.z, sprite_w, 1)) {
            player.speed = camera_speed = 25;
            if (!dont_change_pos) {
                add_to_log("You broke a decoration (+1)");
                add_star();
            }
            dont_change_pos = true;
        }
        else {
            dont_change_pos = false;
        }
    }
}

void Game::process_overtake(Car& car) {
    int car_segment_id = find_segment_i(car.position);
    Segment& car_segment = segments[car_segment_id];
    
    float car_w = car.sprite.getTextureRect().width * car_segment.scale * scale_to_car_k * car_scale;

    for (int i = 0; i < 10; i++) {
        int prev_segment_id = car_segment_id - i - 1 + segments_buffer_size;
        Segment& prev_segment = segments[prev_segment_id % segments_buffer_size];

        for (std::shared_ptr<Car>& prev_car: prev_segment.cars) {
            float prev_car_w = prev_car->sprite.getTextureRect().width * car_segment.scale * scale_to_car_k * car_scale;
            if (overlap(car.x * car_segment.screen.z, car_w, prev_car->x * car_segment.screen.z, prev_car_w, 1)) {
                if (prev_car->lane_id == 2) {
                    prev_car->lane_id = 1;
                }
                else if (prev_car->lane_id == 1) {
                    prev_car->lane_id = 2;
                }
                prev_car->x = get_lane_x(prev_car->lane_id);
            }
        }
    }
}

void Game::process_speed_limit() {
    if (segments[find_segment_i(player.position)].speed_limit * 1.2 <= camera_speed) {
        auto found_ptr = std::lower_bound(map.begin(), map.end(), find_segment_i(player.position) + 1, [](MapSegment& a, int d) -> bool { return a.length_prefix < d; });
        if (!found_ptr->speed_limit_failed) {
            add_star();

            add_to_log("You failed speed limit (+1)");
            found_ptr->speed_limit_failed = true;
        }
    }
}

void Game::update(float dt) {
    process_keypress(dt);
    update_cars(dt);
    process_collisions();
    process_overtake(player);
    process_speed_limit();
}

void Game::process_time() {
    if (timer.getElapsedTime().asSeconds() > max_seconds) {
        *result_ptr = 1;
    }
}

void Game::process_won() {
    if (find_segment_i(camera_position + 5 * segment_length) == 0) {
        lap_count++;
    }
    if (lap_count >= laps_to_win) {
        *result_ptr = 2;
    }
}

void Game::render_player() {
    player.x = camera_x / road_width;
    player.position = camera_position + segment_length * 11;

    int start_segment_i = find_segment_i(player.position);
    Segment& s1 = segments[start_segment_i];
    Segment& s2 = segments[(start_segment_i + 1) % segments_buffer_size];

    bool mirrored = false;
    if (camera_x_speed < 0) {
        mirrored = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        mirrored = false;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        mirrored = true;
    }

    draw_sprite(*window, player.sprite, s1, s2, player.position, player.x, player_scale, mirrored);
}

void Game::render(sf::Event event) {

    window->draw(background);

    process_won();
    process_time();

    int start_segment_i = (int)(camera_position / segment_length) % segments_buffer_size;

    float x = 0;
    float dx = 0;
    float max_y = window_height;
      
    float segment_percent = get_segment_percent(camera_position);

    float segment_y_offset = interpolate(segments[start_segment_i].world.y, segments[(start_segment_i + 1) % segments_buffer_size].world.y, segment_percent);
    float camera_height = camera_height_offset + segment_y_offset;

    dx = -segments[start_segment_i].curve * segment_percent;

    for (int i = start_segment_i; i < start_segment_i + draw_distance; i++) {
        Segment& s1 = segments[i % segments_buffer_size];
        
        sf::Vector3f camera(camera_x - x, camera_height, camera_position - (i >= segments_buffer_size ? segments_buffer_size * segment_length : 0));
        s1.project(camera);

        x += dx;
        dx += s1.curve;

        s1.clip = max_y;

        if (start_segment_i + 1 == i || start_segment_i == i) continue;

        if (s1.screen.y >= max_y) continue;
        max_y = s1.screen.y;

        Segment& s2 = segments[(i - 1 + segments_buffer_size) % segments_buffer_size];

        // Grass
        draw_quad(*window, sf::Vector3f(0, s1.screen.y, window_width), sf::Vector3f(0, s2.screen.y, window_width), (i / 3) % 2 ? grass1_color : grass2_color);

        // Rumble
        draw_quad(*window, sf::Vector3f(s1.screen.x, s1.screen.y, s1.screen.z * rumble_width_k), sf::Vector3f(s2.screen.x, s2.screen.y, s2.screen.z * rumble_width_k), (i / 3) % 2 ? rumble1_color : rumble2_color);

        // Segment
        draw_quad(*window, s1.screen, s2.screen, (i / 3) % 2 ? road1_color : road2_color);

        // Markup
        if ((i / 6) % 2 == 0) {
            float l1 = s1.screen.z * lane_width_k;
            float l2 = s2.screen.z * lane_width_k;
            float y1 = s1.screen.y;
            float y2 = s2.screen.y;
            float w1 = s1.screen.z;
            float w2 = s2.screen.z;
            float lanew1 = w1 * 2 / total_lanes;
            float lanew2 = w2 * 2 / total_lanes;
            float lanex1 = s1.screen.x - w1 + lanew1;
            float lanex2 = s2.screen.x - w2 + lanew2;
            for (int lane_i = 1; lane_i < total_lanes; lanex1 += lanew1, lanex2 += lanew2, lane_i++) {
                sf::ConvexShape shape(4);
                shape.setFillColor(lane_color);
                shape.setPoint(0, sf::Vector2f(lanex1 - l1 / 2, y1));
                shape.setPoint(1, sf::Vector2f(lanex1 + l1 / 2, y1));
                shape.setPoint(2, sf::Vector2f(lanex2 + l2 / 2, y2));
                shape.setPoint(3, sf::Vector2f(lanex2 - l2 / 2, y2));
                window->draw(shape);
            }
        }
    }

    for (int i = start_segment_i + draw_distance; i > start_segment_i; i--) {
        Segment& s1 = segments[i % segments_buffer_size];

        draw_sprite(*window, s1.sprite, s1, s1, i * segment_length, s1.sprite_x, car_scale, s1.sprite_x < 0);

        if (start_segment_i + 1 == i || start_segment_i == i) continue;
        if ((i + 1) % segments_buffer_size == 0 || (i + 1) == (start_segment_i + draw_distance)) continue;

        Segment& s2 = segments[(i + 1) % segments_buffer_size];

        for (int car_i = 0; car_i < s1.cars.size(); car_i++) {
            Car& car = *s1.cars[car_i];

            draw_sprite(*window, car.sprite, s1, s2, car.position, car.x, car_scale, s1.curve < 0.0);
        }
    }

    render_player();
    
    render_info();
    render_log();
    render_stars();

    window->display();
    process_collisions();
}
