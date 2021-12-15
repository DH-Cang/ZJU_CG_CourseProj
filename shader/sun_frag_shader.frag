#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 color;

void main()
{
    if(FragPos.z < 0.0f)
    {
        FragColor = vec4(vec2(color), 1.0f, 0.0f);
    }else{
        vec3 viewDir = normalize(FragPos);
        FragColor = vec4(color, 1.0f);
    }


}