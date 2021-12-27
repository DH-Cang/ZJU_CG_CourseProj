#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

// material data structure declaration
struct Material {
	vec4 ka;
	vec4 kd;
	vec4 ks;
	float shininess;
	sampler2D texture_diffuse1;
};
// uniform variables
uniform Material material;

void main() {
	// ambient color
	FragColor = texture(material.texture_diffuse1, TexCoords);
};