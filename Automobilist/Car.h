#pragma once

#include <SFML/Graphics.hpp>

struct Car {
    float x, position;
    float speed;
    float max_speed;
    float x_speed, wanted_x;
    int lane_id;
    sf::Sprite sprite;
};
 
