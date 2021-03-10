#ifndef CAMERA_H
#define CAMERA_H

#include "math.h"

struct ThirdPersonCamera
{
    ThirdPersonCamera(Vec3* player_position, float* player_rotation);
    ThirdPersonCamera(Vec3* player_position, float* player_rotation, float sensitivity, float distance);

    Vec3 pos;
    Vec3 up;
    float sensitivity;
    float distance;
    Vec3* player_position;
    float* player_rotation;
    Matrix view;
};

void process_camera_movement(ThirdPersonCamera* camera, float deltaTime);


#endif

