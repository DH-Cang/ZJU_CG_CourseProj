#pragma once
#define PI 3.1415926f

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Object3D {
public:
	glm::vec3 position = { 0.0f, 0.0f, 0.0f };
	// q = ( cos(theta/2), (x, y, z)sin(theta/2)) 
	// ÈÆÖá£¨x£¬y£¬z£©Ðý×ªtheta½Ç
	glm::quat rotation = { 1.0f, 0.0f, 0.0f, 0.0f };
	glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

	glm::vec3 getFront() const;

	glm::vec3 getUp() const;

	glm::vec3 getRight() const;

	glm::mat4 getModelMatrix() const;
};