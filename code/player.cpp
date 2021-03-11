#include "player.h"
#include <math.h>
#include <windows.h>

#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689

void init_player(Player* player, Vec3 position, uint32_t shaderId, float speed, float rotation)
{
    player->position = position;
    player->new_position = position;
    player->direction = {0.0f, 0.0f, 1.0f};
    player->speed = speed;
    player->rotation = rotation;
    LoadOBJFileIndex(&player->mesh, "./data/naruto.obj", "./data/naruto.bmp");
    Matrix rotation_mat = get_rotation_x_matrix(to_radiant(-90.0f));
    Vec3 player_scale = {0.01f, 0.01f, 0.01f};
    Matrix scale = get_scale_matrix(player_scale);
    Matrix translation = get_translation_matrix(position);
    player->mesh.model = scale * rotation_mat * translation;
    push_to_render(&player->mesh, 1, shaderId);

    Vec3 min_max_offset = {0.5f, 0.0f, 0.5f};
    player->min = player->new_position - min_max_offset;
    player->max = player->new_position + min_max_offset; 
    player->bounding_box.min = &player->min;
    player->bounding_box.max = &player->max;
}

void process_player_movement(Player* player, float deltaTime)
{
    player->new_position = player->position;
    player->direction.x = player->current_speed_x * deltaTime;
    player->direction.z = player->current_speed_y * deltaTime;
    player->direction = vec3_rotate_y(player->direction, to_radiant(player->current_rotation));
    player->new_position += player->direction;

    Vec3 min_max_offset = {0.5f, 0.0f, 0.5f};
    player->min = player->new_position - min_max_offset;
    player->max = player->new_position + min_max_offset; 
    
    Matrix translation = get_translation_matrix(player->position);
    Matrix rotation_x = get_rotation_x_matrix(to_radiant(-90.0f));
    Matrix rotation_y = get_rotation_y_matrix(to_radiant(player->current_rotation));
    Matrix rotation_mat = rotation_x * rotation_y;
    Vec3 player_scale = {0.01f, 0.01f, 0.01f};
    Matrix scale = get_scale_matrix(player_scale);
    player->mesh.model = scale * rotation_mat * translation;  
}

void player_input_handler(Player* player, Controller* controller)
{
    if(check_for_conected_controller())
    {
        process_xinput(controller);
        float x_left_thumb = 0.0f; 
        float y_left_thumb = 0.0f;       
        if(controller->thumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
        {
             x_left_thumb = controller->thumbLX / 32768.0f;
        }
        else if(controller->thumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
        {
            x_left_thumb = controller->thumbLX / 32768.0f;

        }
        if(controller->thumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
        {
             y_left_thumb = controller->thumbLY / 32768.0f;
        }
        else if(controller->thumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
        {
            y_left_thumb = controller->thumbLY / 32768.0f;

        }

        player->current_speed_y = player->speed * y_left_thumb;
        player->current_speed_x = player->speed * x_left_thumb;

        float x_right_thumb = 0.0f; 
        float y_right_thumb = 0.0f;       
        if(controller->thumbRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
        {
             x_right_thumb = controller->thumbRX / 32768.0f;
        }
        else if(controller->thumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
        {
            x_right_thumb = controller->thumbRX / 32768.0f;

        }
        if(controller->thumbRY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
        {
             y_right_thumb = controller->thumbRY / 32768.0f;
        }
        else if(controller->thumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
        {
            y_right_thumb = controller->thumbRY / 32768.0f;

        }

        player->current_rotation += player->rotation * x_right_thumb;
      
    }
}

void player_handle_colitions(Player* player, Vec3* colition_point, int num_checks)
{
    bool is_coliding = false;
    for(int i = 0; i < num_checks; i++)
    {
        if(player->bounding_box.is_point_inside(colition_point[i]))
        {
            is_coliding = true;      
        }
    }
    if(!is_coliding)
        player->position = player->new_position;
}

