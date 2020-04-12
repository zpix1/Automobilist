#pragma once

#include <SFML/Graphics.hpp>

#include "Car.h"

struct Segment {
    float scale;
    float curve;
    float sprite_x, clip;
    sf::Sprite sprite;

    // X, Y, Z
    sf::Vector3f world;

    // X, Y, W
    sf::Vector3f screen;

    std::vector< std::shared_ptr<Car> > cars;

    Segment();
    void project(sf::Vector3f& camera);
};

