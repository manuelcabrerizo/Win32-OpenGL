#include "terrain.h"
#include <glad/glad.h>

void generate_terrain(Terrain* terrain, Vec3 position, int num_row, int num_col, int cell_spacing, const char* texture_path)
{
    terrain->position      = position;
    terrain->num_vert_row  = num_row;
    terrain->num_vert_col  = num_col;
    terrain->num_cell_row  = num_row - 1;
    terrain->num_cell_col  = num_col - 1;
    terrain->cell_spacing  = cell_spacing;
    terrain->num_vertices  = num_row * num_col;
    terrain->num_triangle  = 2 * (terrain->num_cell_row * terrain->num_cell_col);
    terrain->mesh.numIndex = terrain->num_triangle * 3;
    terrain->mesh.vertexBuffer = (VertexBuffer*)malloc(terrain->num_vertices * sizeof(VertexBuffer));
    terrain->mesh.indices       = (int*)malloc((terrain->num_triangle * 3) * sizeof(int));

    for(int z = 0; z < terrain->num_vert_col; z++)
    {
        for(int x = 0; x < terrain->num_vert_row; x++)
        {
            float position_x = terrain->position.x + ((float)x * (float)terrain->cell_spacing);
            float position_y = 0.0f;
            float position_z = terrain->position.z + ((float)z * (float)terrain->cell_spacing);
            Vec3 vertex = {position_x, position_y, position_z};
            terrain->mesh.vertexBuffer[(z * terrain->num_vert_row) + x].vertice = vertex;
            Vec2 tex_coord;
            tex_coord.x = x * 1.0f / 2; //* (1.0f / terrain->num_cell_row);
            tex_coord.y = z * 1.0f / 2; //* (1.0f / terrain->num_cell_col);
            terrain->mesh.vertexBuffer[(z * terrain->num_vert_row) + x].textureCoord = tex_coord;

        }
    }

    for(int z = 0; z < terrain->num_vert_col; z++)
    {
        for(int x = 0; x < terrain->num_vert_row; x++)
        {
            float a = 0.0f;
            float b = 0.0f;
            float c = 0.0f;
            Vec3 u = {(float)terrain->cell_spacing, b - a, 0.0f};
            Vec3 v = {0.0f, c - a, (float)terrain->cell_spacing};
            Vec3 normal = normaliza_vec3(vec3_cross(u, v));
            terrain->mesh.vertexBuffer[(z * terrain->num_vert_row) + x].normal = -normal;
        }
    }
    
    int index = 0;
    for(int z = 0; z < terrain->num_cell_col; z++)
    {
        for(int x = 0; x < terrain->num_cell_row; x++)
        {
            terrain->mesh.indices[index + 0] = ((z + 1) * terrain->num_vert_row) + x;
            terrain->mesh.indices[index + 1] = (z * terrain->num_vert_row) + (x + 1);
            terrain->mesh.indices[index + 2] = (z * terrain->num_vert_row) + x;

            terrain->mesh.indices[index + 3] = ((z + 1) * terrain->num_vert_row) + (x + 1);
            terrain->mesh.indices[index + 4] = (z * terrain->num_vert_row) + (x + 1);
            terrain->mesh.indices[index + 5] = ((z + 1) * terrain->num_vert_row) + x;
            index += 6;
        }
    }

    glGenVertexArrays(1, &terrain->mesh.vao);
    glBindVertexArray(terrain->mesh.vao);
    uint32_t VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, terrain->num_vertices * sizeof(VertexBuffer), terrain->mesh.vertexBuffer, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexBuffer), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexBuffer), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexBuffer), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    uint32_t EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (terrain->num_triangle * 3) * sizeof(uint32_t), terrain->mesh.indices, GL_STATIC_DRAW);

    uint32_t texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    terrain->mesh.texId = texture1;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    terrain->mesh.tex = LoadBMP(texture_path);

    if(terrain->mesh.tex.pixels != NULL)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, terrain->mesh.tex.width, terrain->mesh.tex.height,
                                    0, GL_BGR, GL_UNSIGNED_BYTE, terrain->mesh.tex.pixels);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("ERROR::LOADING::BMP::FILE\n");
    }
    terrain->mesh.model = get_identity_matrix();

    glBindVertexArray(0);
    free(terrain->mesh.tex.pixels);
    free(terrain->mesh.vertexBuffer);
    free(terrain->mesh.indices);    
}
