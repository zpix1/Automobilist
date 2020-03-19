#pragma once

#include <SFML/Graphics.hpp>

const float fps = 60.0;
const float step = 1 / fps;
const int width = 1024;
const int height = 768;

const sf::Color background = sf::Color::Green;

const float road_width = 2000.0;
const float segment_length = 200.0;
const int rumble_length = 3;
//const float track_length = 0;
const float FOV = 100.0;
const int lanes_n = 3;
const float camera_height = 1000.0;
const int draw_distance = 300;
const float camera_depth = 1.0 / tan(FOV / 2.0);
const float fogDensity = 5;
const float max_speed = segment_length / step;
const float accel = max_speed / 5;
const float breaking = -max_speed;
const float decel = -max_speed / 5;
const float off_road_decel = -max_speed / 2;
const float off_road_limit = max_speed / 4;

const int segments_buffer_size = 500;
