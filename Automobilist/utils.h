#pragma once

#include <SFML/Graphics.hpp>

const float fps = 60.0;
const float step = 1 / fps;
const int width = 1024;
const int height = 768;

const sf::Color background = sf::Color(135, 206, 250);

const sf::Color road1_color = sf::Color(107, 107, 107);
const sf::Color road2_color = sf::Color(105, 105, 105);

const sf::Color rumble1_color = sf::Color(255, 255, 255);
const sf::Color rumble2_color = sf::Color(0, 0, 0);

const sf::Color grass1_color = sf::Color(16, 200, 16);
const sf::Color grass2_color = sf::Color(0, 154, 0);

const float road_width = 3000.0;
const float segment_length = 400.0;
const float rumble_width_k = 1.1;
const float s_camera_height = 2500.0;
const int draw_distance = 200;
const float camera_depth = 0.84;

const float max_x_speed = 3000;
const float fogDensity = 5;
const float max_speed = 400;
const float accel = max_speed / 3;
const float breaking = max_speed;
const float decel = max_speed / 7;
const float off_road_decel = max_speed / 1.3;
const float off_road_limit = max_speed / 8;
const float max_reverse_speed = max_speed / 8;
const float centrifugal = 0.3;

const int total_cars = 10;

const int segments_buffer_size = 1600;

const int amount_of_textures = 3;

void draw_quad(sf::RenderWindow& window, sf::Vector3f screen_top, sf::Vector3f screen_bottom, sf::Color color);