#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;
uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;

void main()
{    
    //FragColor = texture(diffuse_texture, TexCoords);
    FragColor = vec4(0, 0, 0, 1.0f);
}