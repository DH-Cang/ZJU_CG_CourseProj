#include "mesh.h"

Mesh::~Mesh() {
	if (_ebo != 0) {
		glDeleteBuffers(1, &_ebo);
		_ebo = 0;
	}

	if (_vbo != 0) {
		glDeleteBuffers(1, &_vbo);
		_vbo = 0;
	}

	if (_vao != 0) {
		glDeleteVertexArrays(1, &_vao);
		_vao = 0;
	}
}

Mesh::Mesh(
	std::vector<Vertex> _vertices,
	std::vector<uint32_t> _indices,
	struct Material _material,
	std::string diff_tex_name,
	std::string spec_tex_name
) : _vertices(_vertices), _indices(_indices), _material(_material)
{
	if (diff_tex_name.back() == '/') {	// if this is a directory instead of file
		_diffuse_texture.reset();
	}
	else {
		_diffuse_texture.reset(new Texture2D(diff_tex_name));
	}

	if (spec_tex_name.back() == '/') {
		_specular_texture.reset();
	}
	else {
		_specular_texture.reset(new Texture2D(spec_tex_name));
	}

	initGLResources();
}


void Mesh::draw(Shader& shader) const
{
	// set up material
	shader.setVec3("material.ambient", _material.ambient);
	shader.setVec3("material.diffuse", _material.diffuse);
	shader.setVec3("material.specular", _material.specular);
	shader.setFloat("material.shininess", _material.shininess);

	// set up texture
	if (_diffuse_texture.get() != nullptr) {
		shader.setInt("diffuse_texture", 0);
		glActiveTexture(GL_TEXTURE0);
		_diffuse_texture->bind();
	}
	if (_specular_texture.get() != nullptr) {
		shader.setInt("specular_texture", 1);
		glActiveTexture(GL_TEXTURE1);
		_specular_texture->bind();
	}
	
	//bind vao and draw
	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, (GLsizei)_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


void Mesh::initGLResources() {
	// create a vertex array object
	glGenVertexArrays(1, &_vao);
	// create a vertex buffer object
	glGenBuffers(1, &_vbo);
	// create a element array buffer
	glGenBuffers(1, &_ebo);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _vertices.size(), _vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(uint32_t), _indices.data(), GL_STATIC_DRAW);

	// specify layout, size of a vertex, data type, normalize, sizeof vertex array, offset of the attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}