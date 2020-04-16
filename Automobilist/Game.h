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
    void add_to_log(std::string str);
    void render_player();


    sf::Texture& get_texture(std::string name);
public:
    float fps;

    Game(sf::RenderWindow* w);
    void render(sf::Event event);
    void update(float dt);
};

