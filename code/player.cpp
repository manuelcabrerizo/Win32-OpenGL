#include "player.h"
#include <math.h>

#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689

void init_player(Player* player, Vec3 position, uint32_t shaderId, float speed, float rotation)
{
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
}

void process_player_movement(Player* player, float deltaTime)
{

    player->direction.x = player->current_speed_x * deltaTime;
    player->direction.z = player->current_speed_y * deltaTime;
    player->direction = vec3_rotate_y(player->direction, to_radiant(player->current_rotation));
    player->position += player->direction; 
    
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





/*
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

        float magnitude = sqrt(x_left_thumb * x_left_thumb + y_left_thumb * y_left_thumb);
        float norm_x_left_thumb = x_left_thumb / magnitude;
        float norm_y_left_thumb = y_left_thumb / magnitude;

        player->current_speed_y = player->speed * norm_y_left_thumb;
        player->current_speed_x = player->speed * norm_x_left_thumb;

        float x_right_thumb = 0.0f; 
        float y_right_thumb = 0.0f;       
        if(controller->thumbRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
        {
             x_right_thumb = controller->thumbRX / 32768.0f;
        }
        else if(controller->thumbLX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
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

        float magnitude_r = sqrt(x_right_thumb * x_right_thumb + y_right_thumb * y_right_thumb);
        float norm_x_right_thumb = x_left_thumb / magnitude_r;
        float norm_y_right_thumb = y_left_thumb / magnitude_r;

        player->current_rotation += player->rotation * norm_x_right_thumb;
      
    }
}

 
void process_player_movement(Player* player, float deltaTime)
{
    float new_x = (player->current_speed * sinf(to_radiant(player->current_rotation + player->horizontal_rotation))) * deltaTime;
    float new_z = (player->current_speed * cosf(to_radiant(player->current_rotation + player->horizontal_rotation))) * deltaTime;
    player->position = {player->position.x + new_x,
                        0.0f,
                        player->position.z +new_z};
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
        float x_left_thumb = controller->thumbLX;
        float y_left_thumb = controller->thumbLY;
        float magnitude = sqrt(x_left_thumb * x_left_thumb + y_left_thumb * y_left_thumb);
        float norm_x_left_thumb = 0.0f;
        float norm_y_left_thumb = 0.0f;
        if(magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
        {
            if(magnitude > 32767)
            {
                magnitude = 32767;
            }
            magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
        }
        else
        {
            magnitude = 0.0f;
        }
        
        if(magnitude != 0.0f)
        {  
            norm_x_left_thumb = x_left_thumb / magnitude;
            norm_y_left_thumb = y_left_thumb / magnitude;
        }
        else
        {
            norm_x_left_thumb = 0.0f;
            norm_y_left_thumb = 0.0f;
        }

        if(norm_y_left_thumb > 0.5f)
        {
            player->current_speed = player->speed; 
        }
        else if(norm_y_left_thumb < -0.5f)
        {
            player->current_speed = -player->speed;
        }
        else
        {
            player->current_speed = 0.0f;
        }

        if(norm_x_left_thumb > 0.5f)
        {
            player->current_rotation += player->rotation * norm_x_left_thumb;  
        }
        else if(norm_x_left_thumb < -0.5f)
        {
            player->current_rotation += player->rotation * norm_x_left_thumb;
        }
            
    }
}














        if(norm_y_left_thumb > 0.5f)
        {
            player->horizontal_rotation = 0.0f;
            player->current_speed = player->speed; 
        }
        
        if(norm_y_left_thumb < -0.5f)
        {
            player->horizontal_rotation = 0.0f;
            player->current_speed = -player->speed;
        }

        if(norm_x_left_thumb > 0.5f)
        {

            player->horizontal_rotation = 90.0f;
            player->current_speed = player->speed;  
        }
        
        if(norm_x_left_thumb < -0.5f)
        {
            player->horizontal_rotation = 90.0f;
            player->current_speed = -player->speed;  
        }

        if(norm_x_left_thumb == 0.0f && norm_y_left_thumb == 0.0f)
        {
            player->current_speed = 0.0f;
        }




*/
