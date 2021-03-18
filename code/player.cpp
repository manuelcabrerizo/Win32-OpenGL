#include "player.h"
#include <math.h>
#include "renderer.h"
#include <windows.h>

#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689

static float g_deltaTime = 0.0f;

void init_player(Player* player, Vec3 position, uint32_t shaderId, float speed, float rotation)
{
    player->shaderId = shaderId;
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
    player->bounding_box.min = player->min;
    player->bounding_box.max = player->max;
}

void process_player_movement(Player* player, float deltaTime)
{
    g_deltaTime = deltaTime;
    player->new_position = player->position;
    player->direction.x  = player->current_speed_x * deltaTime;
    player->direction.z  = player->current_speed_y * deltaTime;
    player->direction    = vec3_rotate_y(player->direction, to_radiant(player->current_rotation));
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

        player->current_rotation += (player->rotation * x_right_thumb) * g_deltaTime;
    
        if(!controller->x_was_press)
        {
            if(controller->X == true)
            {
                controller->x_was_press = true;
                shot_fireball(player);
            }
        }
        if(controller->X == false)
        {
            controller->x_was_press = false;
        }
    }
}

void player_handle_colitions(Player* player, BoundingBox* bounding_box, int num_checks_box)
{
    bool is_coliding = false;
    Vec3 actual_contat_normal;
    int contact_cout = 0;
    for(int i = 0; i < num_checks_box; i++)
    {
        Vec3 contact_normal;
        Vec3 contact_point;
        float t_hit_near;

        if(ray_intersect_bounding_box(player->new_position, player->direction, bounding_box[i], contact_point, contact_normal, t_hit_near))
        {
            if(t_hit_near <= 1.0f)
            {
                is_coliding = true;
                actual_contat_normal = contact_normal;
                contact_cout++;
            }
        }
    }
    if(is_coliding)
    {
        if(contact_cout == 1)
        {
            if(actual_contat_normal.x > 0 || actual_contat_normal.x < 0)
                player->position.z = player->new_position.z;
            if(actual_contat_normal.z > 0 || actual_contat_normal.z < 0)
                player->position.x = player->new_position.x;
        }
    }
    if(!is_coliding)
    {
        player->position = player->new_position;
    }
}

void init_player_projectil(Player* player, Mesh* mesh, int number_of_bullets, int distance_to_travel, float speed)
{
    player->number_of_fireballs = number_of_bullets;
    player->current_num_of_fireballs = number_of_bullets;
    player->fireballs = (Fireball*)malloc(number_of_bullets * sizeof(Fireball));
    for(int i = 0; i < number_of_bullets; i++)
    {
        player->fireballs[i].position = player->position;
        player->fireballs[i].distance_traveled = 0.0f;
        player->fireballs[i].max_distance = distance_to_travel;
        player->fireballs[i].speed = speed;
        player->fireballs[i].mesh = &mesh[i]; 
    } 
    push_to_render_get_fire_ref(mesh, number_of_bullets, player->shaderId, player->fireballs);
    for(int i = 0; i < number_of_bullets; i++)
    {
        *player->fireballs[i].should_render = false;
    }

}

void update_fireballs(Player* player, float deltaTime)
{
    for(int i = 0; i < player->number_of_fireballs; i++)
    {
        if(*player->fireballs[i].should_render == true)
        {
            player->fireballs[i].distance_traveled += 10.0f * deltaTime;
            float new_pos_x = sinf(to_radiant(player->fireballs[i].current_rotation));
            float new_pos_z = cosf(to_radiant(player->fireballs[i].current_rotation));
            player->fireballs[i].direction.x = new_pos_x;
            player->fireballs[i].direction.z = new_pos_z;
            player->fireballs[i].direction.y = 0.0f;
            player->fireballs[i].position += (player->fireballs[i].direction * player->fireballs[i].speed * deltaTime); 
            Matrix transform = get_translation_matrix(player->fireballs[i].position);
            Vec3 bulle_scale = {0.3f, 0.3f, 0.3f};
            Matrix scale = get_scale_matrix(bulle_scale);
            player->fireballs[i].mesh->model = scale * transform;
        }

        if(player->fireballs[i].distance_traveled > player->fireballs[i].max_distance)
        {
            *player->fireballs[i].should_render = false;
        }
    }
}

void shot_fireball(Player* player)
{
    int index = player->current_num_of_fireballs - 1;
    player->fireballs[index].distance_traveled = 0.0f;
    player->fireballs[index].position = player->position;
    player->fireballs[index].position.y = player->position.y + 1.0f;
    player->fireballs[index].current_rotation = player->current_rotation;
    *player->fireballs[index].should_render = true;
    
    player->current_num_of_fireballs--;
    if(player->current_num_of_fireballs < 1)
    {
        player->current_num_of_fireballs = player->number_of_fireballs;    
    }
}

void clear_fireballs(Player* player, int number_of_bullets)
{
    free(player->fireballs);
}





