#include "Game.h"

#include <string>

void Game::load_textures() {
    if (!main_font.loadFromFile("./assets/Xenogears.ttf")) {
        fprintf(stderr, "[ERROR] Can't load font\n");
    }
    
    std::vector<std::string> tnames = { "bluecar", "palm", "20sign", "60sign", "70sign", "120sign", "xsign", "sky" };
    for (int i = 0; i < tnames.size(); i++) {
        TextureT temp_texture;
        
        if (!temp_texture.texture.loadFromFile("./assets/" + tnames[i] + ".png")) {
            fprintf(stderr, "[ERROR] Can't load texture textures/%d.png", i);
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
    int global_i = 0;
    const int length_k = 3;
    struct mapseg {
        int length;
        float speed_limit;
        std::string texture_name;
        float curve;
    } map[] {
        {50, INFINITY, "xsign", 4.5},
        {300, 60, "60sign", 0.5},
        {600, INFINITY, "xsign", 0},
        {300, 60, "60sign", -3.0},
        {600, 120, "120sign", -4},
        {200, 20, "20sign", -0.3}
    };

    for (int i = 0; i < sizeof(map) / sizeof(map[0]); i++) {
        for (int j = 0; j < map[i].length * length_k; j++, global_i++) {
            Segment segment;
            segment.world.z = global_i * segment_length;
            segment.speed_limit = map[i].speed_limit / speed_to_screen;
            segment.curve = map[i].curve;
            segment.world.y = segment.world.y = sin((global_i) / 30.0) * 1500;
            if (j == 0) {
                segment.sprite_x = 1.5;
                segment.sprite.setTexture(get_texture(map[i].texture_name));
            }
            segments.push_back(segment);
        }
    }



    //for (int i = 0; i < segments_buffer_size; i++) {
    //    Segment segment;

    //    segment.world.z = i * segment_length;
    //    int k = 3;
    //    // length, speed limit, sign texture id, curve
    //    

    //    //// Add hills
    //    //if (i > 0) {
    //    //    segment.world.y = sin((i) / 30.0) * 1500;
    //    //}
    //    //// Add curves
    //    //switch ((i / 300) % 3) {
    //    //    case 0:
    //    //        segment.curve = 3.0;
    //    //        break;
    //    //    case 1:
    //    //        segment.curve = 3.0;
    //    //        break;
    //    //    case 2:
    //    //        segment.curve = -5.7;
    //    //        break;
    //    //}
    //    //// Add sprites
    //    //if (i % 10 == 0) {
    //    //    segment.sprite_x = 1.0;
    //    //    segment.sprite.setTexture(textures[2]);
    //    //}

    //    /*if (i == 100) {
    //        segment.sprite_x = 1.5;
    //        segment.sprite.setTexture(textures[0]);
    //    }*/

    //    segments.push_back(segment);
    //}
    //segments_buffer_size = segments.size();
}

void Game::reset_cars() {
    // Remove old cars
    cars.clear();

    for (int i = 0; i < total_cars; i++) {
        Car car;
        int lane_id = randint(0, total_lanes - 1);
        
        car.lane_id = lane_id;
        car.x = get_lane_x(lane_id) + 0.1 * ((((float)rand() / RAND_MAX)) - 0.5);
        car.position = 100 * i * segment_length;
        car.speed =  (200.0 + 150.0 * ((float)rand() / RAND_MAX));
        car.sprite.setTexture(get_texture("bluecar"));

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
    sprintf_s(info_text, "%.1f\nspeed: %0.1fkm/s / %f\nposition: %0.1f\npx: %f\nsegment_i(cam): %d", fps, camera_speed * speed_to_screen, max_speed, camera_position, camera_x, find_segment_i(camera_position));
    info_panel.setString(info_text);
    window->draw(info_panel);
}

void Game::add_to_log(std::string str) {
    log.push_back(str);

    if (log.size() == 11) {
        log.erase(log.begin());
    }
}

void Game::render_log() {
    sf::Text info_panel;
    info_panel.setCharacterSize(20);
    info_panel.setFillColor(sf::Color(0, 0, 0));
    info_panel.setPosition(window_width - 300, 0);
    info_panel.setFont(main_font);
    std::string info_text;
    for (int i = log.size() - 10; i < log.size(); i++) {
        info_text += log[i] + "\n";
    }
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
        Segment& s1 = segments[find_segment_i(c->position)];
        auto to_delete = std::find(s1.cars.begin(), s1.cars.end(), c);
        s1.cars.erase(to_delete);
        
        c->position += c->speed;

        while (c->position >= segments_buffer_size * segment_length) c->position -= segments_buffer_size * segment_length;
        while (c->position < 0) c->position += segments_buffer_size * segment_length;

        Segment& s2 = segments[find_segment_i(c->position)];
        s2.cars.push_back(c);
    }
}

void Game::process_collisions() {
    Segment& s = segments[find_segment_i(player.position)];

    float player_w = player.sprite.getTextureRect().width * s.scale * scale_to_car_k * player_scale;

    for (auto c : s.cars) {
        if (camera_speed >= c->speed) {
            float car_w = c->sprite.getTextureRect().width * s.scale * scale_to_car_k * car_scale;

            if (overlap(player.x * s.screen.z, player_w, c->x * s.screen.z, car_w, 1)) {
                player.speed = camera_speed = 25;
            }
        }
    }
    {
        float sprite_w = s.sprite.getTextureRect().width * s.scale * scale_to_car_k * car_scale;
        if (sprite_w)
        if (overlap(player.x * s.screen.z, player_w, s.sprite_x * s.screen.z, sprite_w, 1)) {
            player.speed = camera_speed = 25;
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
        char exp[100];
        sprintf_s(exp, "you failed speed limit, %.1f > %.1f", camera_speed * speed_to_screen, segments[find_segment_i(player.position)].speed_limit * speed_to_screen);
        camera_speed = segments[find_segment_i(player.position)].speed_limit * 0.8;
        std::string s = exp;
        add_to_log(s);
    }
}

void Game::update(float dt) {
    process_keypress(dt);
    update_cars(dt);
    process_collisions();
    process_overtake(player);
    process_speed_limit();
}

void Game::render_player() {
    player.x = camera_x / road_width;
    player.position = camera_position + segment_length * 6;

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

    render_info();
    render_log();

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

        draw_sprite(*window, s1.sprite, s1, s1, i * segment_length, s1.sprite_x, car_scale, false);

        if (start_segment_i + 1 == i || start_segment_i == i) continue;
        if ((i + 1) % segments_buffer_size == 0 || (i + 1) == (start_segment_i + draw_distance)) continue;

        Segment& s2 = segments[(i + 1) % segments_buffer_size];

        for (int car_i = 0; car_i < s1.cars.size(); car_i++) {
            Car& car = *s1.cars[car_i];

            draw_sprite(*window, car.sprite, s1, s2, car.position, car.x, car_scale, s1.curve < 0.0);
        }
    }

    render_player();

    window->display();


    process_collisions();
}
