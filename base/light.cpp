#include "light.h"

SunLight::SunLight(const float& localLat, const float& subsolarLatitude): subsolarLatitude(glm::radians(subsolarLatitude)), subsolarLongitude(0)
{
	float theta = acos(0) - glm::radians(localLat);
	rotateMat = { 1.0f, 0.0f, 0.0f,
				0.0f, cos(theta), -sin(theta),
				0.0f, sin(theta),  cos(theta)};
	innerXYZ = { CELESTIAL_RADIUS * cos(this->subsolarLatitude) * (-1) * sin(this->subsolarLongitude),
				CELESTIAL_RADIUS * sin(this->subsolarLatitude),
				CELESTIAL_RADIUS * cos(this->subsolarLatitude) * cos(this->subsolarLongitude)
	};
	position = innerXYZ * rotateMat;
	glm::vec3 top = { 0.0f, 1.0f, 0.0f };
	theta = acos(glm::dot(top, position) / glm::length(position));
	elevationAngle = acos(0) - theta;
};

glm::vec3 SunLight::getPos()
{
	return position;
}

void SunLight::updateLight(const float& deltaTime)
{
	subsolarLongitude += deltaTime * angularVelocity;
	innerXYZ = { CELESTIAL_RADIUS * cos(this->subsolarLatitude) * (-1) * sin(this->subsolarLongitude),
			CELESTIAL_RADIUS * sin(this->subsolarLatitude),
			CELESTIAL_RADIUS * cos(this->subsolarLatitude) * cos(this->subsolarLongitude)
	};
	position =  innerXYZ * rotateMat;
	glm::vec3 top = { 0.0f, 1.0f, 0.0f };
	float theta = acos(glm::dot(top, position) / glm::length(position));
	elevationAngle = acos(0) - theta;
	color = glm::vec3(pow(sin(elevationAngle + CIVILIAN_TWILIGHT_ANGLE), 0.01f),
		pow(sin(elevationAngle + 0.2 * CIVILIAN_TWILIGHT_ANGLE), 0.15f),
		pow(sin(elevationAngle + 0.2 * CIVILIAN_TWILIGHT_ANGLE), 0.3f));

}

float SunLight::getElevationAngle()
{
	return elevationAngle;
}