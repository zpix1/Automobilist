#include "Segment.h"
#include "utils.h"

Segment::Segment() {
    world = sf::Vector3f(0, 0, 0);
    screen = sf::Vector3f(0, 0, 0);

    curve = 0;
    scale = 0;
    sprite_x = 0;
    clip = 0;
}

void Segment::project(sf::Vector3f& camera) {
    scale = camera_depth / (world.z - camera.z);
    screen.x = (1.0 + scale * (world.x - camera.x)) * window_width / 2.0;
    screen.y = (1.0 - scale * (world.y - camera.y)) * window_height / 2.0;
    screen.z = scale * road_width * window_width / 2.0;
}