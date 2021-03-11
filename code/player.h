#ifndef PLAYER_H
#define PLAYER_H

#include "math.h"
#include "renderer.h"
#include "controller.h"

struct Player
{
    Vec3 position;
    Vec3 new_position;
    Vec3 direction;
    float speed;
    float current_speed_x = 0;
    float current_speed_y = 0;
    float current_rotation = 0;
    float rotation;
    Mesh mesh;
    Vec3 min;
    Vec3 max;
    BoundingBox bounding_box;
};

void init_player(Player* player, Vec3 position, uint32_t shaderId, float speed, float rotation);
void process_player_movement(Player* player, float deltaTime);
void player_input_handler(Player* player, Controller* controller);
void player_handle_colitions(Player* player, Vec3* colition_point, int num_checks);

#endif
