#pragma once
#include "object3d.h"
#define CELESTIAL_RADIUS 1000.0f // 太阳距离，恰好在该距离上的物体可能会穿模
#define CIVILIAN_TWILIGHT_ANGLE 0.09f // 以弧度计民用晨昏蒙影


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

	float radius = 20.0f;

private:
	const float angularVelocity = 0.06f;

	const float sunIntensityBase = 15.0f;

	float subsolarLatitude;

	float subsolarLongitude;

	glm::vec3 innerXYZ;

	glm::mat3 rotateMat;
	
	float elevationAngle;


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