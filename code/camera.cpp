#include "camera.h"
#include <math.h>
#include <windows.h>
#include <stdio.h>
ThirdPersonCamera::ThirdPersonCamera(Vec3* player_position, float* player_rotation)
{
    this->pos = {0.0f, 5.0f, -10.0f};
    this->up  = {0.0f, 0.1f, 0.0f};
    this->sensitivity = 0.1f;
    this->distance = 10.0f;
    this->player_position = player_position;
    this->player_rotation = player_rotation;
}

ThirdPersonCamera::ThirdPersonCamera(Vec3* player_position, float* player_rotation, float sensitivity, float distance)
{
    this->pos;
    this->up  = {0.0f, 0.1f, 0.0f};
    this->player_position = player_position;
    this->player_rotation = player_rotation;
    this->sensitivity = sensitivity;
    this->distance = distance;
}

void process_camera_movement(ThirdPersonCamera* camera, float deltaTime)
{
    Vec3 up = {0.0f, 1.0f, 0.0f};
    float new_x = (camera->distance * sinf(to_radiant(*camera->player_rotation)));
    float new_z = (camera->distance * cosf(to_radiant(*camera->player_rotation)));
    camera->pos.x = camera->player_position->x - new_x;
    camera->pos.z = camera->player_position->z - new_z; 
    Vec3 target = {camera->player_position->x, camera->player_position->y + 2.0f, camera->player_position->z};
    camera->view = get_view_matrix(camera->pos, target, up);
}


