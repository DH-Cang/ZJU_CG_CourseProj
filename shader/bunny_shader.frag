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
};

// directional light data structure declaration
struct DirectionalLight {
	vec3 direction;
	float intensity;
	vec3 color;
};

// uniform variables
uniform Material material;
uniform DirectionalLight directionalLight;
uniform vec3 eyes;

void main() {
	// several variables
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(FragPos - eyes);
	vec3 lightDir = normalize(-directionalLight.direction);
	vec3 reflectDir = reflect(lightDir, normal);

	// diffuse
	vec3 diffuse = directionalLight.intensity * directionalLight.color * max(dot(lightDir, normal), 0.0f) * vec3(material.kd);

	// specular
	vec3 specular = directionalLight.intensity * directionalLight.color * pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess) * vec3(material.ks);

	// ambient
	vec3 ambient = vec3(material.ka);

	// output
	FragColor = vec4(diffuse + specular + ambient, 1.0f);
}