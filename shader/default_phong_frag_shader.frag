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
	sampler2D texture_specular1;
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

//对函数进行了封装，一些全局变量可能没有改成私有变量

vec4 getDirectionalColor(vec3 n, DirectionalLight dl) {
	vec3 normal = normalize(n);
	vec3 viewDir = normalize(FragPos - eyes);
	vec3 lightDir = normalize(-dl.direction);
	vec3 reflectDir = reflect(lightDir, normal);
	// diffuse and specular color
	vec3 diffuse = dl.intensity * dl.color * max(dot(lightDir, normal), 0.0f) * vec3(material.kd);
	vec3 specular = dl.intensity * dl.color * pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess) * vec3(material.ks);

	vec4 ret = 7.0f * texture(material.texture_diffuse1, TexCoords) * vec4(diffuse, 1.0f) + 
				3.0f * texture(material.texture_specular1, TexCoords) * vec4(specular, 1.0f);
	return ret;

}


void main() {
		
	// ambient color
	vec3 ambient = vec3(material.ka);
	FragColor = getDirectionalColor(Normal, directionalLight) + vec4(ambient, 1.0f) + 0.3f * texture(material.texture_diffuse1, TexCoords);
};