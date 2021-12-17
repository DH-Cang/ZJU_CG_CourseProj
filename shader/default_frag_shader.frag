#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

struct Material{
    vec4 ka;
    vec4 kd;
    vec4 ks;
    float shininess;
    sampler2D texture_diffuse1;
};

uniform Material material;

void main()
{    
    FragColor = texture(material.texture_diffuse1, TexCoords).rgba;
    //FragColor = vec4(0, 0, 0, 1.0f);
    //FragColor = material.kd;
}