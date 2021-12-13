	#version 330 core
	in vec3 FragPos;
	in vec3 Normal;
	out vec4 color;

	// material data structure declaration
	struct Material {
		vec3 albedo;
		float ka;
		vec3 kd;
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
	uniform sampler2D mapKd;

	vec3 calcDirectionalLight(vec3 normal) {
		vec3 lightDir = normalize(-directionalLight.direction);
		// diffuse color
		vec3 diffuse = directionalLight.color * max(dot(lightDir, normal), 0.0f) * material.kd;
		return directionalLight.intensity * diffuse ;
	}

	vec3 calcSpotLight(vec3 normal) {
		vec3 lightDir = normalize(spotLight.position - FragPos);
		float theta = acos(-dot(lightDir, normalize(spotLight.direction)));
		if (theta > spotLight.angle) {
			return vec3(0.0f, 0.0f, 0.0f);
		}
		vec3 diffuse = spotLight.color * max(dot(lightDir, normal), 0.0f) * material.kd;
		float distance = length(spotLight.position - FragPos);
		float attenuation = 1.0f / (spotLight.kc + spotLight.kl * distance + spotLight.kq * distance * distance);
		return spotLight.intensity * attenuation * diffuse;
	}
		
	void main() {
		vec3 normal = normalize(Normal);
		// ambient color
		vec3 ambient = material.ka * material.albedo;
		// diffuse color
		vec3 diffuse = calcDirectionalLight(normal) + calcSpotLight(normal);
		color = vec4(ambient + diffuse, 1.0f);
	};