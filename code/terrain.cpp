#include "terrain.h"
#include <glad/glad.h>
#include "renderer.h"
#include "enemy.h"

static BoundingBox* test_box;
static int g_num_quads = 0;
static int g_num_enemies = 0;

static Mesh tree;
static Mesh* trees;

static Mesh enemy_bullet;
static Mesh** enemy_bullets;
static Enemy* enemies;

static int map[400] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
    1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 
    1, 0, 0, 1, 0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 1, 0, 0, 0, 0, 1, 
    1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 
    1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 
    1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 
    1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 
    1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 
    1, 0, 0, 0, 0, 2, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 
    1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1,
    1, 1, 1, 0, 1, 1, 1, 1, 0, 2, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 
    1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 
    1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 2, 0, 1, 0, 0, 0, 1, 0, 0, 1, 
    1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 
    1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 
    1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 
    1, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 
    1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 
};

void setup_terrain(Player* player, Terrain* terrain, Shader mesh_shader)
{
    int number_of_quads = 0;
    int number_of_enemies = 0;
    for(int y = 0; y < 20; y++)
    {
        for(int x = 0; x < 20; x++)
        {
            if(map[(y * 20) + x] == 1)
            {
                number_of_quads++;
            }
            if(map[(y * 20) + x] == 2)
            {
                number_of_enemies++;
            }
        }
    }
    g_num_quads = number_of_quads;
    g_num_enemies = number_of_enemies;
    test_box = (BoundingBox*)malloc(number_of_quads * sizeof(BoundingBox));

    trees = (Mesh*)malloc((number_of_quads * 16) * sizeof(Mesh));
    LoadOBJFileIndex(&tree, "./data/tree.obj", "./data/tree4.bmp");
    for(int i = 0; i < number_of_quads * 16; i++)
    {
        trees[i] = tree;
    }
    push_to_render(trees, number_of_quads * 16, mesh_shader.ID);

    LoadOBJFileIndex(&enemy_bullet, "./data/bullet.obj", "./data/bullet-enemy.bmp"); 
    enemies = (Enemy*)malloc(number_of_enemies * sizeof(Enemy));
    enemy_bullets = (Mesh**)malloc(number_of_enemies * sizeof(Mesh*));
    for(int i = 0; i < number_of_enemies; i++)
    {
        enemy_bullets[i] = (Mesh*)malloc(10 * sizeof(Mesh));
        for(int j = 0; j < 10; j++)
        {
            enemy_bullets[i][j] = enemy_bullet;
        }
    }

    int index = 0;
    int index_enemy = 0;
    for(int y = 0; y < 20; y++)
    {
        for(int x = 0; x < 20; x++)
        {
            if(map[(y * 20) + x] == 1)
            {
                test_box[index].min = {((float)x * terrain->cell_spacing) - 2.0f, 0.0f, ((float)y * terrain->cell_spacing) - 2.0f};
                test_box[index].max = {((float)x * terrain->cell_spacing) + 2.1f, 0.0f, ((float)y * terrain->cell_spacing) + 2.1f};
                for(int i = 0; i < 4; i++)
                {
                    for(int j = 0; j < 4; j++)
                    {
                        Vec3 current_tree_pos = {((float)x * terrain->cell_spacing + i) - 1.5f, 0.0f, ((float)y * terrain->cell_spacing + j) - 1.5f};
                        trees[(index * 16) + ((i * 4) + j)].model = get_translation_matrix(current_tree_pos);
                    }
                }
                index++;
            }
            
            if(map[(y * 20) + x] == 2)
            {
                Vec3 enemy_pos = {(float)x * terrain->cell_spacing, 1.0f, (float)y * terrain->cell_spacing};
                enemies[index_enemy] = Enemy(); 
                init_enemy(&enemies[index_enemy], enemy_pos, mesh_shader.ID, 200);
                init_enemy_projectil(&enemies[index_enemy], enemy_bullets[index_enemy], 10, 20, 10.0f); 
                index_enemy++;
            }
        }
    }

}

void terrain_coilitions(Player* player, float deltaTime)
{
    player_handle_colitions(player, test_box, g_num_quads);
    for(int i = 0; i < g_num_enemies; i++)
    {
        update_enemy(&enemies[i], &player->position, deltaTime);
    }
}

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
            tex_coord.x = x * 1.0f / 2; 
            tex_coord.y = z * 1.0f / 2; 
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
