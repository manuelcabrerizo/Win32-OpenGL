#version 330 core

out vec4 FragColor;
in vec2 TexCoord;
in vec3 normal;

uniform sampler2D texture1;

void main()
{
    vec3 light_dir = vec3(1.0, 0.0, 0.0);
    vec3 norm = normalize(normal);
    float shadow = dot(norm, light_dir);
    if(shadow <= 0.0)
    {
        shadow = 0.0;
    }
    vec3 objectColor = texture(texture1, TexCoord).xyz;
    FragColor = (vec4(objectColor, 1.0) * (shadow + 0.3));
}
