#pragma once
#include "object3d.h"
#define FARTHEST 20000.0f // Ïà»úÔ¶¶Ë
#define CELESTIAL_RADIUS (0.9f*FARTHEST) // Ì«Ñô¾àÀë

class Light : public Object3D {
public:
	Light() = default;

	~Light() = default;

public:
	float intensity = 1.0f;
	glm::vec3 color = { 1.0f, 1.0f, 1.0f };
};

class DirectionalLight : public Light{
public:
	DirectionalLight() = default;

	~DirectionalLight() = default;
};

class SunLight : public DirectionalLight {
public:
	SunLight(const float& localLat, const float& subsolarLatitude);

	~SunLight() {};

	glm::vec3 getPos();

	void updateLight(const float& deltaTime);

	float getElevationAngle();

private:
	const float angularVelocity = 0.2f;

	float subsolarLatitude;

	float subsolarLongitude;

	glm::vec3 innerXYZ;

	glm::mat3 rotateMat;

};

class SpotLight : public Light {
public:
	SpotLight() = default;

	~SpotLight() = default;

public:
	float angle = glm::radians(60.0f);
	float kc = 1.0f;
	float kl = 0.0f;
	float kq = 0.2f;
};