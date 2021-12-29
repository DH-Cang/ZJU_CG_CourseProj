#pragma once
#include "shader.h"
#include "mesh.h"
#include "object3d.h"
#include "manualtexture.h"
#include "light.h"
#include "collision.h"
#include <vector>
#include <string>
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <fstream>
#include <sstream>
#include <iostream>

struct ver {
	glm::vec3 pos;
	glm::vec3 normal;
};


class ObjModel : public Object3D
{
public:
	ObjModel(string const& path);
	ObjModel() = default;
	~ObjModel();

	void Draw(Shader& shader);

private:
	GLuint vao, vbo;
	std::vector<struct ver> vertices;	// x, y, z, nx, ny, nz

	void initGL(void);

};
