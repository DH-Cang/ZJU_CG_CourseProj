#pragma once

#include <string>
#include <vector>

#include <glad/glad.h>

#include "mesh.h"
#include "object3d.h"


class Model : public Object3D {
public:
	Model(const std::string& filepath);

	~Model();

	Model(Model&& model) noexcept = default;

	void draw(Shader& shader) const;

private:
	// vertices of the table represented in model's own coordinate
	std::vector<Mesh> meshes;
};