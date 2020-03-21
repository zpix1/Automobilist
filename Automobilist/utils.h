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

const float road_width = 2000.0;
const float segment_length = 200.0;
const float rumble_width_k = 1.2;
//const float track_length = 0;
const float FOV = 100.0;
const int lanes_n = 3;
const float camera_height = 1000.0;
const int draw_distance = 300;
const float camera_depth = 1;

const float x_speed = 200;
const float fogDensity = 5;
const float max_speed = 100;
const float accel = max_speed / 5;
const float breaking = -max_speed;
const float decel = max_speed / 5;
const float off_road_decel = -max_speed / 2;
const float off_road_limit = max_speed / 4;

const float centrifugal = 0.3;

const int segments_buffer_size = 1600;

void draw_quad(sf::RenderWindow& window, sf::Vector3f screen_top, sf::Vector3f screen_bottom, sf::Color color);