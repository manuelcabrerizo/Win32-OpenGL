#ifndef PLAYER_H
#define PLAYER_H

#include "utility.h"
#include "shader.h"
#include "math.h"
#include "controller.h"

struct Player
{
    int shaderId;
    Vec3  position;
    Vec3  new_position;
    Vec3  direction;
    float speed;
    float speed_back_up;
    float current_speed_x = 0;
    float current_speed_y = 0;
    float current_rotation = 0;
    float rotation;
    Mesh  mesh;
    Vec3  min;
    Vec3  max;
    BoundingBox bounding_box;
    int number_of_fireballs;
    int current_num_of_fireballs;
    Fireball* fireballs;
};

void init_player(Player* player, Vec3 position, uint32_t shaderId, float speed, float rotation);
void process_player_movement(Player* player, float deltaTime);
void player_input_handler(Player* player, Controller* controller);
void player_handle_colitions(Player* player, BoundingBox* bounding_box, int num_checks_box);
void init_player_projectil(Player* player, Mesh* mesh, int number_of_bullets, int distance_to_travel, float speed);
void update_fireballs(Player* player, float deltaTime);
void shot_fireball(Player* player);

#endif
