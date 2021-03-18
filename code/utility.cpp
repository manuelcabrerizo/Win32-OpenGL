#include <glad/glad.h>
#include "utility.h"
#include <string.h>


void swap(float& a, float& b)
{
    float temp = a;
    a = b;
    b = temp;
}

float maxf(float a, float b)
{
    if(a > b)
        return a;
    else
        return b;
}

float minf(float a, float b)
{
    if(a < b)
        return a;
    else
        return b;
}

float absf(float a)
{
    if(a < 0)
    {
        a *= -1.0f;
    }
    return a;
}

Vec3 abs_vec3(Vec3 v)
{
    Vec3 result;
    result.x = absf(v.x);
    result.y = absf(v.y);
    result.z = absf(v.z);
    return result;
}


bool ray_intersect_bounding_box(Vec3 ray_origin, Vec3 ray_dir, BoundingBox target,
                                Vec3& contact_point, Vec3& contact_normal, float& t_hit_near)
{
    float target_width = target.max.x - target.min.x;
    float target_depth = target.max.z - target.min.z;
    float target_x = target.min.x;
    float target_z = target.min.z;
    Vec3 target_pos = {target_x, 0.0f, target_z};
    Vec3 target_size = {target_width, 0.0f, target_depth};

    Vec3 t_near = (target_pos - ray_origin) / ray_dir;
    Vec3 t_far  = (target_pos + target_size - ray_origin) / ray_dir;

    if(t_near.x > t_far.x)
        swap(t_near.x, t_far.x);
    if(t_near.z > t_far.z)
        swap(t_near.z, t_far.z);
    if(t_near.x > t_far.z || t_near.z > t_far.x)
        return false;
    
    t_hit_near = maxf(t_near.x, t_near.z);
    float t_hit_far = minf(t_far.x, t_far.z);
    if(t_hit_far < 0) 
       return false;

    Vec3 result = ray_dir * t_hit_near;
    contact_point = ray_origin + result;

    if(t_near.x > t_near.z)
    {
        if(ray_dir.x < 0)
            contact_normal = {1.0f, 0.0f, 0.0f};
        else
            contact_normal = {-1.0f, 0.0f, 0.0f};
    }
    else if(t_near.x < t_near.z)
    {
        if(ray_dir.z < 0)
            contact_normal = {0.0f, 0.0f, 1.0f};
        else
            contact_normal = {0.0f, 0.0f, -1.0f};
    } 

    return true;
}


bool BoundingBox::is_point_inside(Vec3& p)
{
    if(p.x >= this->min.x && p.y >= this->min.y && p.z >= this->min.z &&
       p.x <= this->max.x && p.y <= this->max.y && p.z <= this->max.z)
    {
        return true;
    }   
    else
    {
        return false;
    }
}

bool BoundingBox::is_bounding_box_inside(BoundingBox& box)
{
    float box_width = box.max.x - box.min.x;
    float box_depth = box.max.z - box.min.z;
    float box_x = box.min.x;
    float box_z = box.min.z;

    float player_width = this->max.x - this->min.x;
    float player_depth = this->max.z - this->min.z;
    float player_x = this->min.x;
    float player_z = this->min.z;

    if(player_x <= box_x + box_width &&
       player_x + player_width >= box_x &&
       player_z <= box_z + box_depth &&
       player_z + player_depth >+ box_z)
    {
        return true;
    }
    else
    {
        return false;
    }
}

Vec3 get_middle_of_bounding_box(BoundingBox box)
{
    float box_width = box.max.x - box.min.x;
    float box_depth = box.max.z - box.min.z;
    float box_x = box.min.x;
    float box_z = box.min.z;
    Vec3 middle = {box_x + (box_width / 2.0f), 0.01f, box_z + (box_depth / 2.0f)};
    return middle;
}

Vec3 get_scale_of_bounding_box(BoundingBox box)
{
    Vec3 result = {
        box.max.x - box.min.x,
        0.0f,
        box.max.z - box.min.z
    };
    return result;
}

void setup_quad(Mesh* quad)
{

    float quad_vertices[] = {
        0.5f, 0.1f,  0.5f, 
        0.5f, 0.1f, -0.5f, 
        -0.5f, 0.1f, -0.5f, 
        -0.5f, 0.1f,  0.5f  
    };
    unsigned int quad_indices[] = {  
        0, 1, 3,   
        1, 2, 3    
    };

    // setup quad stuff
    quad->vertexBuffer = (VertexBuffer*)malloc(4 * sizeof(VertexBuffer));
    quad->indices      = (int*)malloc(6 * sizeof(int));

    int index = 0;
    for(int i = 0; i < 4; i++)
    {
        quad->vertexBuffer[i].vertice.x = quad_vertices[index];
        index++;
        quad->vertexBuffer[i].vertice.y = quad_vertices[index];
        index++;
        quad->vertexBuffer[i].vertice.z = quad_vertices[index];
        index++;

        Vec3 nulo_vec3 = {0.0f, 0.0f, 0.0f};
        Vec2 nulo_vec2 = {0.0f, 0.0f};
        quad->vertexBuffer[i].textureCoord = nulo_vec2;
        quad->vertexBuffer[i].normal = nulo_vec3;
    }

    for(int i = 0; i < 6; i++)
    {
        quad->indices[i] = quad_indices[i]; 
    }
    quad->numIndex = 6;  

    glGenVertexArrays(1, &quad->vao);
    glBindVertexArray(quad->vao);
    uint32_t VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(VertexBuffer), quad->vertexBuffer, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexBuffer), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexBuffer), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexBuffer), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    uint32_t EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, quad->numIndex * sizeof(unsigned int), quad->indices, GL_STATIC_DRAW);

    quad->model = get_identity_matrix();

    glBindVertexArray(0);
    free(quad->vertexBuffer);
    free(quad->indices); 
}

Texture LoadBMP(const char* filename)
{
    bitmapHeader header;
    Texture texture;
    FILE* file = fopen(filename, "rb");
    if(file != NULL)
    {
        fread(&header, sizeof(header), 1, file);
        texture.pixels = (uint32_t*)malloc(sizeof(uint32_t) * header.width * header.height);
        fseek(file, header.bitmapOffset, SEEK_SET);
        fread(texture.pixels, sizeof(uint32_t), header.width * header.height, file);
        texture.width = header.width;
        texture.height = header.height;
    }
    else
    {
        printf("ERROR::LOADING::BMP::FILE: %s\n", filename);
    }
    fclose(file);
    return(texture);
}


void LoadOBJFile(Mesh* mesh, const char* filePhat, const char* texFileName)
{
    mesh->numVertices  = 0;
    mesh->numIndex     = 0;
    mesh->numTexCoords = 0;
    mesh->numNormals   = 0;

    FILE* file;
    file = fopen(filePhat, "r");

    if(file == NULL)
    {
        printf("ERROR::LOADING::OBJ::FILE\n");
    }
    char line[1024];

    while(fgets(line, 1024, file) != NULL)
    {    
        // first we have to count the size 
        // becouse we need to allocate memory for
        // the obj object 
        if(strncmp(line, "v ", 2) == 0)
        {
            mesh->numVertices++;
        }
        if(strncmp(line, "vt ", 3) == 0)
        {
            mesh->numTexCoords++;
        }
        if(strncmp(line, "vn ", 3) == 0)
        {
            mesh->numNormals++;
        }
        if(strncmp(line, "f ", 2) == 0)
        {
            mesh->numIndex++;
        }
    } 
    // we allocate memory for the model 
    mesh->vertices      = (Vec3*)malloc(mesh->numVertices  * sizeof(Vec3));
    mesh->textureCoords = (Vec2*)malloc(mesh->numTexCoords * sizeof(Vec2));
    mesh->normals       = (Vec3*)malloc(mesh->numNormals   * sizeof(Vec3));
    mesh->vertexBuffer  = (VertexBuffer*)malloc((mesh->numIndex * 3) * sizeof(VertexBuffer));
    mesh->vertexIndex   = (IndexBuffer*)malloc(mesh->numIndex * sizeof(IndexBuffer));
    mesh->textureIndex  = (IndexBuffer* )malloc(mesh->numIndex * sizeof(IndexBuffer));
    mesh->normalIndex   = (IndexBuffer*)malloc(mesh->numIndex * sizeof(IndexBuffer));
    
    Vec3 test;
    rewind(file);
    int vertexIndex = 0;
    int textIndex = 0;
    int normalIndex = 0;
    int indexIndex = 0;
    while(fgets(line, 1024, file) != NULL)
    {     
        if(strncmp(line, "v ", 2) == 0)
        {
            float x, y, z;
            sscanf(line, "v %f %f %f", &x, &y, &z);
            mesh->vertices[vertexIndex].x = x;
            mesh->vertices[vertexIndex].y = y;
            mesh->vertices[vertexIndex].z = z; 
            vertexIndex++;
        }
        if(strncmp(line, "vt ", 3) == 0)
        {
            float x, y;
            sscanf(line, "vt %f %f", &x, &y);
            mesh->textureCoords[textIndex].x = x;
            mesh->textureCoords[textIndex].y = y;
            textIndex++;
        }
        if(strncmp(line, "vn ", 3) == 0)
        {
            float x, y, z;
            sscanf(line, "vn %f %f %f", &x, &y, &z);
            mesh->normals[normalIndex].x = x;
            mesh->normals[normalIndex].y = y;
            mesh->normals[normalIndex].z = z;
            normalIndex++;
        }
        if(strncmp(line, "f ", 2) == 0)
        {
            int indices[3];
            int textures[3];
            int normals[3];
            sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                &indices[0], &textures[0], &normals[0],
                &indices[1], &textures[1], &normals[1],
                &indices[2], &textures[2], &normals[2]);

            mesh->vertexIndex[indexIndex].a = indices[0];
            mesh->vertexIndex[indexIndex].b = indices[1];
            mesh->vertexIndex[indexIndex].c = indices[2];

            mesh->textureIndex[indexIndex].a = textures[0];
            mesh->textureIndex[indexIndex].b = textures[1];
            mesh->textureIndex[indexIndex].c = textures[2];

            mesh->normalIndex[indexIndex].a = normals[0];
            mesh->normalIndex[indexIndex].b = normals[1];
            mesh->normalIndex[indexIndex].c = normals[2];
            indexIndex++;
        }
    }
    
    // with all the obj info we have to fill the vertex and index buffer
    int indexCounter = 0;
    for(int i = 0; i < (mesh->numIndex * 3); i += 3)
    {
        // vertice
        mesh->vertexBuffer[i].vertice.x = mesh->vertices[mesh->vertexIndex[indexCounter].a - 1].x;
        mesh->vertexBuffer[i].vertice.y = mesh->vertices[mesh->vertexIndex[indexCounter].a - 1].y;
        mesh->vertexBuffer[i].vertice.z = mesh->vertices[mesh->vertexIndex[indexCounter].a - 1].z;
        mesh->vertexBuffer[i + 1].vertice.x = mesh->vertices[mesh->vertexIndex[indexCounter].b - 1].x;
        mesh->vertexBuffer[i + 1].vertice.y = mesh->vertices[mesh->vertexIndex[indexCounter].b - 1].y;
        mesh->vertexBuffer[i + 1].vertice.z = mesh->vertices[mesh->vertexIndex[indexCounter].b - 1].z;
        mesh->vertexBuffer[i + 2].vertice.x = mesh->vertices[mesh->vertexIndex[indexCounter].c - 1].x;
        mesh->vertexBuffer[i + 2].vertice.y = mesh->vertices[mesh->vertexIndex[indexCounter].c - 1].y;
        mesh->vertexBuffer[i + 2].vertice.z = mesh->vertices[mesh->vertexIndex[indexCounter].c - 1].z;
        // texture Coords
        mesh->vertexBuffer[i].textureCoord.x = mesh->textureCoords[mesh->textureIndex[indexCounter].a - 1].x;
        mesh->vertexBuffer[i].textureCoord.y = mesh->textureCoords[mesh->textureIndex[indexCounter].a - 1].y;
        mesh->vertexBuffer[i + 1].textureCoord.x = mesh->textureCoords[mesh->textureIndex[indexCounter].b - 1].x;
        mesh->vertexBuffer[i + 1].textureCoord.y = mesh->textureCoords[mesh->textureIndex[indexCounter].b - 1].y;
        mesh->vertexBuffer[i + 2].textureCoord.x = mesh->textureCoords[mesh->textureIndex[indexCounter].c - 1].x;
        mesh->vertexBuffer[i + 2].textureCoord.y = mesh->textureCoords[mesh->textureIndex[indexCounter].c - 1].y;
        // normals
        mesh->vertexBuffer[i].normal.x = mesh->normals[mesh->normalIndex[indexCounter].a - 1].x;
        mesh->vertexBuffer[i].normal.y = mesh->normals[mesh->normalIndex[indexCounter].a - 1].y;
        mesh->vertexBuffer[i].normal.z = mesh->normals[mesh->normalIndex[indexCounter].a - 1].z;
        mesh->vertexBuffer[i + 1].normal.x = mesh->normals[mesh->normalIndex[indexCounter].b - 1].x;
        mesh->vertexBuffer[i + 1].normal.y = mesh->normals[mesh->normalIndex[indexCounter].b - 1].y;
        mesh->vertexBuffer[i + 1].normal.z = mesh->normals[mesh->normalIndex[indexCounter].b - 1].z;
        mesh->vertexBuffer[i + 2].normal.x = mesh->normals[mesh->normalIndex[indexCounter].c - 1].x;
        mesh->vertexBuffer[i + 2].normal.y = mesh->normals[mesh->normalIndex[indexCounter].c - 1].y;
        mesh->vertexBuffer[i + 2].normal.z = mesh->normals[mesh->normalIndex[indexCounter].c - 1].z;
        indexCounter++;
    }
    
    glGenVertexArrays(1, &mesh->vao);
    glBindVertexArray(mesh->vao);
    uint32_t verticesVBO;
    glGenBuffers(1, &verticesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
    glBufferData(GL_ARRAY_BUFFER, (mesh->numIndex*3)*sizeof(VertexBuffer), mesh->vertexBuffer, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexBuffer), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexBuffer), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexBuffer), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    free(mesh->vertices); 
    free(mesh->textureCoords);
    free(mesh->normals); 
    free(mesh->vertexBuffer);
    free(mesh->vertexIndex);
    free(mesh->textureIndex);
    free(mesh->normalIndex);
 
    uint32_t texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    mesh->texId = texture1;
    // test loadd bmp file:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    mesh->tex = LoadBMP(texFileName);

    if(mesh->tex.pixels != NULL)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mesh->tex.width, mesh->tex.height,
                                    0, GL_BGR, GL_UNSIGNED_BYTE, mesh->tex.pixels);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("ERROR::LOADING::BMP::FILE\n");
    }
    free(mesh->tex.pixels);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindVertexArray(0);

    mesh->model = get_identity_matrix();
}

void LoadOBJFileIndex(Mesh* mesh, const char* filePhat, const char* texFileName)
{
    mesh->numVertices  = 0;
    mesh->numIndex     = 0;
    mesh->numTexCoords = 0;
    mesh->numNormals   = 0;

    FILE* file;
    file = fopen(filePhat, "r");

    if(file == NULL)
    {
        printf("ERROR::LOADING::OBJ::FILE\n");
    }
    char line[1024];

    while(fgets(line, 1024, file) != NULL)
    {    
        // first we have to count the size 
        // becouse we need to allocate memory for
        // the obj object 
        if(strncmp(line, "v ", 2) == 0)
        {
            mesh->numVertices++;
        }
        if(strncmp(line, "vt ", 3) == 0)
        {
            mesh->numTexCoords++;
        }
        if(strncmp(line, "vn ", 3) == 0)
        {
            mesh->numNormals++;
        }
        if(strncmp(line, "f ", 2) == 0)
        {
            mesh->numIndex++;
        }
    } 
    // we allocate memory for the model 
    mesh->vertices      = (Vec3*)malloc(mesh->numVertices  * sizeof(Vec3));
    mesh->textureCoords = (Vec2*)malloc(mesh->numTexCoords * sizeof(Vec2));
    mesh->normals       = (Vec3*)malloc(mesh->numNormals   * sizeof(Vec3));
    mesh->vertexBuffer  = (VertexBuffer*)malloc((mesh->numIndex * 3) * sizeof(VertexBuffer));
    mesh->fVertexBuffer = (VertexBuffer*)malloc(mesh->numVertices * sizeof(VertexBuffer));
    mesh->vertexIndex   = (IndexBuffer*)malloc(mesh->numIndex * sizeof(IndexBuffer));
    mesh->textureIndex  = (IndexBuffer* )malloc(mesh->numIndex * sizeof(IndexBuffer));
    mesh->normalIndex   = (IndexBuffer*)malloc(mesh->numIndex * sizeof(IndexBuffer));
    mesh->indices       = (int*)malloc((mesh->numIndex * 3) * sizeof(int));
    
    Vec3 test;
    rewind(file);
    int vertexIndex = 0;
    int textIndex = 0;
    int normalIndex = 0;
    int indexIndex = 0;
    while(fgets(line, 1024, file) != NULL)
    {     
        if(strncmp(line, "v ", 2) == 0)
        {
            float x, y, z;
            sscanf(line, "v %f %f %f", &x, &y, &z);
            mesh->vertices[vertexIndex].x = x;
            mesh->vertices[vertexIndex].y = y;
            mesh->vertices[vertexIndex].z = z; 
            vertexIndex++;
        }
        if(strncmp(line, "vt ", 3) == 0)
        {
            float x, y;
            sscanf(line, "vt %f %f", &x, &y);
            mesh->textureCoords[textIndex].x = x;
            mesh->textureCoords[textIndex].y = y;
            textIndex++;
        }
        if(strncmp(line, "vn ", 3) == 0)
        {
            float x, y, z;
            sscanf(line, "vn %f %f %f", &x, &y, &z);
            mesh->normals[normalIndex].x = x;
            mesh->normals[normalIndex].y = y;
            mesh->normals[normalIndex].z = z;
            normalIndex++;
        }
        if(strncmp(line, "f ", 2) == 0)
        {
            int indices[3];
            int textures[3];
            int normals[3];
            sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                &indices[0], &textures[0], &normals[0],
                &indices[1], &textures[1], &normals[1],
                &indices[2], &textures[2], &normals[2]);

            mesh->vertexIndex[indexIndex].a = indices[0];
            mesh->vertexIndex[indexIndex].b = indices[1];
            mesh->vertexIndex[indexIndex].c = indices[2];

            mesh->textureIndex[indexIndex].a = textures[0];
            mesh->textureIndex[indexIndex].b = textures[1];
            mesh->textureIndex[indexIndex].c = textures[2];

            mesh->normalIndex[indexIndex].a = normals[0];
            mesh->normalIndex[indexIndex].b = normals[1];
            mesh->normalIndex[indexIndex].c = normals[2];
            indexIndex++;
        }
    }
    
    // with all the obj info we have to fill the vertex and index buffer
    int indexCounter = 0;
    for(int i = 0; i < (mesh->numIndex * 3); i += 3)
    {
        // vertice
        mesh->vertexBuffer[i].vertice.x = mesh->vertices[mesh->vertexIndex[indexCounter].a - 1].x;
        mesh->vertexBuffer[i].vertice.y = mesh->vertices[mesh->vertexIndex[indexCounter].a - 1].y;
        mesh->vertexBuffer[i].vertice.z = mesh->vertices[mesh->vertexIndex[indexCounter].a - 1].z;
        mesh->vertexBuffer[i + 1].vertice.x = mesh->vertices[mesh->vertexIndex[indexCounter].b - 1].x;
        mesh->vertexBuffer[i + 1].vertice.y = mesh->vertices[mesh->vertexIndex[indexCounter].b - 1].y;
        mesh->vertexBuffer[i + 1].vertice.z = mesh->vertices[mesh->vertexIndex[indexCounter].b - 1].z;
        mesh->vertexBuffer[i + 2].vertice.x = mesh->vertices[mesh->vertexIndex[indexCounter].c - 1].x;
        mesh->vertexBuffer[i + 2].vertice.y = mesh->vertices[mesh->vertexIndex[indexCounter].c - 1].y;
        mesh->vertexBuffer[i + 2].vertice.z = mesh->vertices[mesh->vertexIndex[indexCounter].c - 1].z;
        // texture Coords
        mesh->vertexBuffer[i].textureCoord.x = mesh->textureCoords[mesh->textureIndex[indexCounter].a - 1].x;
        mesh->vertexBuffer[i].textureCoord.y = mesh->textureCoords[mesh->textureIndex[indexCounter].a - 1].y;
        mesh->vertexBuffer[i + 1].textureCoord.x = mesh->textureCoords[mesh->textureIndex[indexCounter].b - 1].x;
        mesh->vertexBuffer[i + 1].textureCoord.y = mesh->textureCoords[mesh->textureIndex[indexCounter].b - 1].y;
        mesh->vertexBuffer[i + 2].textureCoord.x = mesh->textureCoords[mesh->textureIndex[indexCounter].c - 1].x;
        mesh->vertexBuffer[i + 2].textureCoord.y = mesh->textureCoords[mesh->textureIndex[indexCounter].c - 1].y;
        // normals
        mesh->vertexBuffer[i].normal.x = mesh->normals[mesh->normalIndex[indexCounter].a - 1].x;
        mesh->vertexBuffer[i].normal.y = mesh->normals[mesh->normalIndex[indexCounter].a - 1].y;
        mesh->vertexBuffer[i].normal.z = mesh->normals[mesh->normalIndex[indexCounter].a - 1].z;
        mesh->vertexBuffer[i + 1].normal.x = mesh->normals[mesh->normalIndex[indexCounter].b - 1].x;
        mesh->vertexBuffer[i + 1].normal.y = mesh->normals[mesh->normalIndex[indexCounter].b - 1].y;
        mesh->vertexBuffer[i + 1].normal.z = mesh->normals[mesh->normalIndex[indexCounter].b - 1].z;
        mesh->vertexBuffer[i + 2].normal.x = mesh->normals[mesh->normalIndex[indexCounter].c - 1].x;
        mesh->vertexBuffer[i + 2].normal.y = mesh->normals[mesh->normalIndex[indexCounter].c - 1].y;
        mesh->vertexBuffer[i + 2].normal.z = mesh->normals[mesh->normalIndex[indexCounter].c - 1].z;
        indexCounter++;
    }

    for(int i = 0; i < mesh->numIndex; i++)
    {
        int index = i * 3;
        mesh->indices[index + 0] = mesh->vertexIndex[i].a - 1;
        mesh->indices[index + 1] = mesh->vertexIndex[i].b - 1;
        mesh->indices[index + 2] = mesh->vertexIndex[i].c - 1;
    }

    for(int i = 0; i < mesh->numIndex * 3; i++)
    {
        mesh->fVertexBuffer[mesh->indices[i]] = mesh->vertexBuffer[i];
    }

    glGenVertexArrays(1, &mesh->vao);
    glBindVertexArray(mesh->vao);
    
    uint32_t verticesVBO;
    glGenBuffers(1, &verticesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
    glBufferData(GL_ARRAY_BUFFER, mesh->numVertices*sizeof(VertexBuffer), mesh->fVertexBuffer, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexBuffer), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexBuffer), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexBuffer), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    uint32_t EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (mesh->numIndex * 3) * sizeof(int), mesh->indices, GL_STATIC_DRAW);

    free(mesh->vertices); 
    free(mesh->textureCoords);
    free(mesh->normals); 
    free(mesh->vertexBuffer);
    free(mesh->vertexIndex);
    free(mesh->textureIndex);
    free(mesh->normalIndex);
    free(mesh->fVertexBuffer);
    free(mesh->indices);
 
    uint32_t texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    mesh->texId = texture1;
    // test loadd bmp file:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    mesh->tex = LoadBMP(texFileName);

    if(mesh->tex.pixels != NULL)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mesh->tex.width, mesh->tex.height,
                                    0, GL_BGR, GL_UNSIGNED_BYTE, mesh->tex.pixels);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("ERROR::LOADING::BMP::FILE\n");
    }
    free(mesh->tex.pixels);

    mesh->model = get_identity_matrix();
}

