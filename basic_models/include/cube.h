#pragma once
#include "../../base/object3d.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>


class Cube : public Object3D {
public:
	Cube(float size);
	~Cube();

	void Draw();

private:
	std::vector<glm::vec3> vertices;

	GLuint vao, vbo, ebo;
};
