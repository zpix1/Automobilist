#pragma once

#include <SFML/Graphics.hpp>

struct Car {
    float x, position;
    float speed;
    float max_speed;
    int lane_id;
    sf::Sprite sprite;
};

