#ifndef TERRAIN_H
#define TERRAIN_H

#include <stdint.h>
#include "math.h"
#include "utility.h"

struct Terrain
{
    Vec3 position;
    uint32_t num_vert_row;
    uint32_t num_vert_col;
    uint32_t num_cell_row;
    uint32_t num_cell_col;
    uint32_t cell_spacing;
    uint32_t num_vertices;
    uint32_t num_triangle;
    Mesh mesh;
};

void generate_terrain(Terrain* terrain, Vec3 position, int num_row, int num_col, int cell_spacing, const char* texture_path);



#endif
