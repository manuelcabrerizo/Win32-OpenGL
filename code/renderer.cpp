#include <windows.h>
#include <glad/glad.h>
#include "renderer.h"
#include <vector>


static std::vector<render_unit> render_buffer;

void push_to_render(Terrain* terrain, uint32_t shaderId)
{
    render_unit meshes;
    meshes.vaoId = terrain->mesh.vao;
    meshes.texId = terrain->mesh.texId;
    meshes.num_meshes = 1;
    meshes.shaderId = shaderId;
    meshes.entities = (Mesh**)malloc(meshes.num_meshes * sizeof(void*));
    *meshes.entities = &terrain->mesh;
    meshes.world_location = glGetUniformLocation(shaderId, "world");
    render_buffer.push_back(meshes);
}

void push_to_render(Mesh* mesh, int num_meshes, uint32_t shaderId)
{
    render_unit meshes;
    meshes.vaoId = mesh->vao;
    meshes.texId = mesh->texId;
    meshes.num_meshes = num_meshes;
    meshes.shaderId = shaderId;
    meshes.entities = (Mesh**)malloc(num_meshes * sizeof(void*));
    meshes.world_location = glGetUniformLocation(shaderId, "world");
    for(int i = 0; i < num_meshes; i++)
    {
        meshes.entities[i] = &mesh[i]; 
    }
    render_buffer.push_back(meshes); 
}

void render(float deltaTime)
{
    for(int i = 0; i < render_buffer.size(); i++)
    {
        shader_use(render_buffer[i].shaderId);
        glBindVertexArray(render_buffer[i].vaoId);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, render_buffer[i].texId);
        for(int j = 0; j < render_buffer[i].num_meshes; j++)
        {
            glUniformMatrix4fv(render_buffer[i].world_location,
                               1, GL_FALSE,
                               render_buffer[i].entities[j]->model.m[0]);
            glDrawElements(GL_TRIANGLES,
                           render_buffer[i].entities[j]->numIndex * 3,
                           GL_UNSIGNED_INT, 0);
        }
    }
}

void clear_render()
{
    for(int i = 0; i < render_buffer.size(); i++)
    {
        free(render_buffer[i].entities);
    }
    render_buffer.clear();
}


