#ifndef ENEMY_H
#define ENEMY_H

#include "renderer.h"
#include "utility.h"
#include "shader.h"
#include "math.h"

struct Enemy
{
    uint32_t shader_id;
    Vec3 position;
    Vec3 direction;
    int number_of_fireballs;
    int current_num_of_fireballs;
    Fireball* fireballs;
    Mesh mesh;
    float timer; 
};

void init_enemy(Enemy* enemy, Vec3 position, uint32_t shader_id, float rotation_speed);
void update_enemy(Enemy* enemy, Vec3* player_pos, float deltaTime);

void init_enemy_projectil(Enemy* enemy, Mesh* mesh, int number_of_bullets, int distance_to_travel, float speed);
void enemy_shoot_fireball(Enemy* enemy, Vec3 enemy_look_direction);


#endif
