#pragma once

#include <vector>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Segment.h"
#include "utils.h"

struct TextureT {
    sf::Texture texture;
    std::string name;
};

struct MapSegment {
    int length;
    int length_prefix;
    float speed_limit;
    std::string texture_name;
    float curve;
    bool speed_limit_failed;
};

class Game {
    sf::RenderWindow* window;
    sf::Font main_font;

    sf::Sprite background;

    Car player;
    std::vector< std::shared_ptr<Car> > cars;

    sf::Texture background_texture;
    std::vector<TextureT> textures;

    std::vector<Segment> segments;

    std::vector<std::string> log;

    float camera_x = 0;
    float camera_position = 0;
    float camera_speed = 0;
    float camera_x_speed = 0;
    bool dont_change_pos = false;

    int stars_count = 0;
    
    void load_textures();
    void fill_segments();
    void reset_cars();

    void process_keypress(float dt);
    void update_cars(float dt);
    void process_collisions();
    void process_overtake(Car& car);
    void process_speed_limit();

    void render_info();
    void render_log();
    void render_stars();
    void add_to_log(std::string str);
    void render_player();

    std::vector<MapSegment> map;

    sf::Texture& get_texture(std::string name);
public:
    float fps;

    Game(sf::RenderWindow* w);
    void render(sf::Event event);
    void update(float dt);
};

