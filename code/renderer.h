#ifndef RENDERER_H
#define RENDERER_H

#include <stdint.h>
#include "utility.h"
#include "shader.h"
#include "terrain.h"
#include "player.h"

struct render_unit
{
    uint32_t vaoId;
    uint32_t texId;
    int num_meshes;
    uint32_t shaderId;
    Mesh** entities;
    unsigned int world_location;
};
void push_to_render(Terrain* terrain, uint32_t shaderId);
void push_to_render(Mesh* mesh, int num_meshes, uint32_t shaderId);
void push_to_render_get_fire_ref(Mesh* mesh, int num_meshes, uint32_t shaderId, Fireball* fireballs);
void render(float deltaTime);
void clear_render();

#endif
