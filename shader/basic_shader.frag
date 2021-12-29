#version 330 core
in vec3 FragPos;
in vec3 Normal;

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

// spot light data structure declaration
struct SpotLight {
	vec3 position;
	vec3 direction;
	float intensity;
	vec3 color;
	float angle;
	float kc;
	float kl;
	float kq;
};

// uniform variables
uniform Material material;

uniform DirectionalLight directionalLight;
uniform SpotLight spotLight;
uniform vec3 eyes;

vec3 calcSpotLight(vec3 normal) {
	vec3 viewDir = normalize(FragPos - eyes);
	vec3 lightDir = normalize(spotLight.position - FragPos);
	vec3 reflectDir = reflect(lightDir, Normal);
	float theta = acos(-dot(lightDir, normalize(spotLight.direction)));
	if (theta > spotLight.angle) {
		return vec3(0.0f, 0.0f, 0.0f);
	}
	vec3 diffuse = spotLight.color * max(dot(lightDir, normal), 0.0f) * vec3(material.kd);
	vec3 specular = spotLight.color * pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess) * vec3(material.ks);
	float distance = length(spotLight.position - FragPos);
	float attenuation = 1.0f / (spotLight.kc + spotLight.kl * distance + spotLight.kq * distance * distance);
	return spotLight.intensity * attenuation * (diffuse + specular);
}


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
	float ambient_strength = 0.5;
	vec3 ambient = ambient_strength * vec3(material.ka);
	vec3 headLight = calcSpotLight(normal);

	// output
	FragColor = vec4(diffuse + specular + ambient + headLight , 1.0f);
}