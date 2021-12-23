#version 330 core

out vec4 FragColor;

// material data structure declaration
struct Material {
	vec4 ka;
	vec4 kd;
	vec4 ks;
	float shininess;
};

// uniform variables
uniform Material material;

void main() {
	FragColor = material.kd;
};