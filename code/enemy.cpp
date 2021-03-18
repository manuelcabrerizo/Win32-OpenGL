#include "enemy.h"
#include <math.h>
#include <windows.h>

void init_enemy(Enemy* enemy, Vec3 position, uint32_t shader_id, float rotation_speed)
{
    enemy->timer = 0.0f;
    enemy->position = position;
    enemy->shader_id = shader_id;
    LoadOBJFileIndex(&enemy->mesh, "./data/drone.obj", "./data/drone.bmp");
    enemy->mesh.model = get_translation_matrix(enemy->position);
    push_to_render(&enemy->mesh, 1, enemy->shader_id);  
}

void update_enemy(Enemy* enemy, Vec3* player_pos, float deltaTime)
{

    Vec3 enemy_look_direction = enemy->position - *player_pos;
    float player_distance = vec3_length(enemy_look_direction);
    Vec3 enemy_default_direction = {0.0f, 0.0f, 1.0f};
    Vec3 enemy_look_direction_norm = normaliza_vec3(enemy_look_direction);
    enemy_default_direction = normaliza_vec3(enemy_default_direction);
    float dot_result = vec3_dot(enemy_default_direction, enemy_look_direction_norm);
    float angle_between = acosf(dot_result);
    if(enemy_look_direction.x < 0.0f)
    {
        angle_between = to_radiant(360.0f) - angle_between;
    }    
    enemy->mesh.model = get_rotation_y_matrix(angle_between) * get_translation_matrix(enemy->position);



    for(int i = 0; i < enemy->number_of_fireballs; i++)
    {
        if(*enemy->fireballs[i].should_render == true)
        {
            enemy->fireballs[i].distance_traveled += 10.0f * deltaTime;
            enemy->fireballs[i].position -= ((enemy->fireballs[i].direction * enemy->fireballs[i].speed)  * deltaTime); 
            Matrix transform = get_translation_matrix(enemy->fireballs[i].position);
            Vec3 bulle_scale = {0.3f, 0.3f, 0.3f};
            Matrix scale = get_scale_matrix(bulle_scale);
            enemy->fireballs[i].mesh->model = scale * transform;
        }

        if(enemy->fireballs[i].distance_traveled > enemy->fireballs[i].max_distance)
        {
            *enemy->fireballs[i].should_render = false;
        }
    }

    enemy->timer += deltaTime;
    if(player_distance < 15.0f && player_distance > 5.0f)
    {
        enemy->position.x -= enemy_look_direction_norm.x / 40.0f;
        enemy->position.z -= enemy_look_direction_norm.z / 40.0f; 
    }
    if(player_distance < 15.0f)
    {
        if(enemy->timer > 0.3f)
        {
            enemy_shoot_fireball(enemy, enemy_look_direction);
            enemy->timer = 0.0f;
        }
    }
}

void init_enemy_projectil(Enemy* enemy, Mesh* mesh, int number_of_bullets, int distance_to_travel, float speed)
{
    enemy->number_of_fireballs = number_of_bullets;
    enemy->current_num_of_fireballs = number_of_bullets;
    enemy->fireballs = (Fireball*)malloc(number_of_bullets * sizeof(Fireball));
    for(int i = 0; i < number_of_bullets; i++)
    {
        enemy->fireballs[i].position = enemy->position;
        enemy->fireballs[i].distance_traveled = 0.0f;
        enemy->fireballs[i].max_distance = distance_to_travel;
        enemy->fireballs[i].speed = speed;
        enemy->fireballs[i].mesh = &mesh[i]; 
    } 
    push_to_render_get_fire_ref(mesh, number_of_bullets, enemy->shader_id, enemy->fireballs);
    for(int i = 0; i < number_of_bullets; i++)
    {
        *enemy->fireballs[i].should_render = false;
    }
}

void enemy_shoot_fireball(Enemy* enemy, Vec3 enemy_look_direction)
{
    int index = enemy->current_num_of_fireballs - 1;
            
    enemy->fireballs[index].distance_traveled = 0.0f;
    enemy->fireballs[index].position = enemy->position;
  
    if((enemy_look_direction.x < 1.0f && enemy_look_direction.x > -1.0f) &&
    (enemy_look_direction.z < 1.0f && enemy_look_direction.z > -1.0f))
    {
        if(enemy_look_direction.x > 0)
        {
            enemy_look_direction.x = 1.0f;
        }
        else
        {
            enemy_look_direction.x = -1.0f;
        }
    
        if(enemy_look_direction.z > 0)
        {
            enemy_look_direction.z = 1.0f;
        }
        else
        {
            enemy_look_direction.z = -1.0f;
        }
    }
    
    enemy_look_direction = normaliza_vec3(enemy_look_direction);

    enemy->fireballs[index].direction.x = enemy_look_direction.x;
    enemy->fireballs[index].direction.z = enemy_look_direction.z;
    enemy->fireballs[index].direction.y = 0.0f;

    *enemy->fireballs[index].should_render = true;
    
    enemy->current_num_of_fireballs--;
    if(enemy->current_num_of_fireballs < 1)
    {
        enemy->current_num_of_fireballs = enemy->number_of_fireballs;    
    }
}
