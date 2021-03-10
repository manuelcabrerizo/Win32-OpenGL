#ifndef PLAYER_H
#define PLAYER_H

#include "math.h"
#include "renderer.h"
#include "controller.h"

struct Player
{
    Vec3 position;
    Vec3 direction;
    float speed;
    float current_speed_x = 0;
    float current_speed_y = 0;
    float current_rotation = 0;
    float rotation;
    Mesh mesh;
};

void init_player(Player* player, Vec3 position, uint32_t shaderId, float speed, float rotation);
void process_player_movement(Player* player, float deltaTime);
void player_input_handler(Player* player, Controller* controller);

#endif
