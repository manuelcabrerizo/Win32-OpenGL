#ifndef UTILITY_H
#define UTILITY_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "math.h"

struct Camera 
{
    Vec3 pos      = {0.0f, 0.0f,  3.0f};
    Vec3 front    = {0.0f, 0.0f,  -1.0f};
    Vec3 up       = {0.0f, 1.0f,  0.0f};
    Vec3 right    = {0.0f, 0.0f,  0.0f};
    Vec3 world_up = {0.0f, 1.0f,  0.0f};
    Vec3 world_front = {0.0f, 0.0f, -1.0f};
    float yaw     = -90.0f;
    float pitch   = 0.0f;
    float speed   = 0.05f;
    float sensitivity = 0.1f;
};

#pragma pack(push, 1)
struct bitmapHeader
{
    uint16_t fileType;
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t bitmapOffset;
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitsPerPixel;
};
#pragma pack(pop)

struct Texture
{
    uint32_t* pixels;
    int width;
    int height;
};

struct IndexBuffer
{
    int a, b, c;
};

struct VertexBuffer
{
    Vec3 vertice; 
    Vec3 normal; 
    Vec2 textureCoord;
};

struct Mesh
{
    uint32_t vao;
    int numVertices;
    int numTexCoords;
    int numNormals;
    int numIndex; 
    Vec3* vertices;
    Vec2* textureCoords;
    Vec3* normals;
    int* indices;
    IndexBuffer* vertexIndex;
    IndexBuffer* textureIndex;
    IndexBuffer* normalIndex;
    VertexBuffer* vertexBuffer;
    VertexBuffer* fVertexBuffer;
    Texture tex;
    uint32_t texId;
    Matrix model;
    bool should_render = true;
};

struct BoundingBox
{
    Vec3 min;
    Vec3 max;
    bool is_point_inside(Vec3& p);
    bool is_bounding_box_inside(BoundingBox& box);
};

struct Fireball
{
    bool* should_render;
    float current_rotation; 
    Vec3  position;
    Vec3  direction;
    float distance_traveled;
    float max_distance;
    float speed;
    Mesh*  mesh;
    int render_id;
};

float maxf(float a, float b);
float minf(float a, float b);
void swap(float& a, float& b);
float absf(float a);
Vec3 abs_vec3(Vec3 v);
bool ray_intersect_bounding_box(Vec3 ray_origin, Vec3 ray_dir, BoundingBox target,
                                Vec3& contact_point, Vec3& contact_normal, float& t_hit_near);
Vec3 get_middle_of_bounding_box(BoundingBox box);
Vec3 get_scale_of_bounding_box(BoundingBox box);
void setup_quad(Mesh* quad);
Texture LoadBMP(const char* filename);
void LoadOBJFile(Mesh* mesh, const char* filePhat, const char* texFileName);
void LoadOBJFileIndex(Mesh* mesh, const char* filePhat, const char* texFileName);

#endif
