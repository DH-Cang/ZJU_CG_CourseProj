#pragma once
#include <string>
#include <vector>

#include <glad/glad.h>

#include "vertex.h"
#include "texture.h"
#include "shader.h"


struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

class Mesh {
public:
	Mesh(
		std::vector<Vertex> _vertices,
		std::vector<uint32_t> _indices,
		struct Material _material,
		std::string diff_tex_name,
		std::string spec_tex_name
	);

	Mesh(Mesh&& mesh) noexcept = default;	// “∆∂Øππ‘Ï

	~Mesh();

	void draw(Shader& shader) const;

private:
	// vertex data
	std::vector<Vertex> _vertices;
	std::vector<uint32_t> _indices;

	// material
	struct Material _material;

	// texture
	std::unique_ptr<Texture2D> _diffuse_texture;
	std::unique_ptr<Texture2D> _specular_texture;

	// opengl object
	GLuint _vao = 0;
	GLuint _vbo = 0;
	GLuint _ebo = 0;

	void initGLResources();
};
