#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 normal;

uniform mat4 world;
uniform mat4 view;
uniform mat4 proj;




void main()
{
    gl_Position = proj * view * world * vec4(aPos, 1.0);
    normal = mat3(transpose(inverse(world))) * aNormal;
    TexCoord = aTexCoord;
}
