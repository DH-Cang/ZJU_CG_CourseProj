#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 WorldCoords;
in vec3 Normal;

vec3 LightCoords = vec3(0.0f, 100.0f, 100.0f);

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
    // ambient
    vec3 ambient = material.ambient;

    // diffuse
    vec3 LightDirect = normalize(LightCoords - WorldCoords);
    vec3 diffuse = material.diffuse * max(dot(normalize(Normal), LightDirect), 0.0f);

    // output
    FragColor = vec4(diffuse, 1.0f);

    //FragColor = texture(diffuse_texture, TexCoords);
    //FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    //FragColor = vec4(material.diffuse, 1.0f);
}