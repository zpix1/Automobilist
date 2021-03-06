#pragma once

#include <SFML/Graphics.hpp>

#include "Segment.h"

const float fps = 60.0;
const float step = 1.0 / fps;
const int window_width = 1024;
const int window_height = 768;

const sf::Color background_color = sf::Color(135, 206, 250);

const sf::Color road1_color = sf::Color(107, 107, 107);
const sf::Color road2_color = sf::Color(105, 105, 105);

const sf::Color rumble1_color = sf::Color(255, 255, 255);
const sf::Color rumble2_color = sf::Color(0, 0, 0);

const sf::Color grass1_color = sf::Color(16, 200, 16);
const sf::Color grass2_color = sf::Color(0, 154, 0);

const sf::Color lane_color = sf::Color(255, 255, 255);

const float road_width = 3000.0;
const float segment_length = 300.0;
const float rumble_width_k = 1.05;
const float lane_width_k = 0.03;
const float camera_height_offset = 2500.0;
const float camera_depth = 0.94;

const float max_x_speed = 4500;
const float max_speed = 500;
const float max_car_speed = 400;
const float acceleration_value = max_speed / 10;
const float breaking_value = max_speed;
const float deceleration_value = max_speed / 15;
const float off_road_deceleration_value = max_speed / 1.3;
const float off_road_max_speed = max_speed / 8;
const float reverse_max_speed = max_speed / 8;
const float centrifugal_k = 0.5;

const float player_scale = 2;
const float car_scale = player_scale;

const float scale_to_car_k = 3000.0;

const int total_cars = 200;

const int total_lanes = 3;

const int segments_buffer_size = 6150;
const int draw_distance = 500;

const float speed_to_screen = 0.25;

const int total_textures = 7;

const int max_seconds = 300;

const int laps_to_win = 2;

void draw_quad(sf::RenderWindow& window, sf::Vector3f screen_top, sf::Vector3f screen_bottom, sf::Color color);

// Should be executed only after segments projections
void draw_sprite(sf::RenderWindow& window, sf::Sprite sprite, Segment& s1, Segment& s2, float position, float x, float scale, bool mirrored);

float interpolate(float a, float b, float percent);

bool overlap(float x1, float w1, float x2, float w2, float percent);

int randint(int min, int max);

int find_segment_i(float position);

float get_segment_percent(float position);

float get_lane_x(int lane_i);

// t is from 0 to 1
float pbend(float t);
