#include <iostream>
#include <unordered_map>

#include <tiny_obj_loader.h>

#include "model.h"

Model::Model(const std::string& filepath) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err, warning;

	std::string::size_type index = filepath.find_last_of("/");
	std::string mtlBaseDir = filepath.substr(0, index + 1);

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warning, &err, filepath.c_str(), mtlBaseDir.c_str())) {
		throw std::runtime_error("load " + filepath + " failure: " + err);
	}

	if (!err.empty()) {
		std::cerr << err << std::endl;
	}

	if (!warning.empty()) {
		std::cout << warning << std::endl;
	}

	//================ analyze data now =============================
	std::vector<struct Material> used_materials;
	std::vector<std::string> diffuse_texture_name;
	std::vector<std::string> specular_texture_name;

	// loop over materials
	for (const auto& material : materials) {
		struct Material reader;
		for (int i = 0; i < 3; i++) {
			reader.ambient[i] = material.ambient[i];
			reader.diffuse[i] = material.diffuse[i];
			reader.specular[i] = material.specular[i];
		}
		reader.shininess = material.shininess;

		used_materials.push_back(reader);
		specular_texture_name.push_back(material.specular_texname);
		diffuse_texture_name.push_back(material.diffuse_texname);
	}

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::unordered_map<Vertex, uint32_t> uniqueVertices;
	struct Material shape_material;
	std::string shape_diff_tex_name;
	std::string shape_spec_tex_name;

	// loop over shapes
	for (const auto& shape : shapes) {
		vertices.clear();
		indices.clear();
		uniqueVertices.clear();
		// loop over vertex index
		for (const auto& index : shape.mesh.indices) {
			Vertex vertex{};

			vertex.position.x = attrib.vertices[3 * index.vertex_index + 0];
			vertex.position.y = attrib.vertices[3 * index.vertex_index + 1];
			vertex.position.z = attrib.vertices[3 * index.vertex_index + 2];

			if (index.normal_index >= 0) {
				vertex.normal.x = attrib.normals[3 * index.normal_index + 0];
				vertex.normal.y = attrib.normals[3 * index.normal_index + 1];
				vertex.normal.z = attrib.normals[3 * index.normal_index + 2];
			}

			if (index.texcoord_index >= 0) {
				vertex.texCoord.x = attrib.texcoords[2 * index.texcoord_index + 0];
				vertex.texCoord.y = attrib.texcoords[2 * index.texcoord_index + 1];
			}

			// check if the vertex appeared before to reduce redundant data
			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}

			indices.push_back(uniqueVertices[vertex]);
		} // loop vertex index end

		// 虽然材质material是按面存储的，但是一个shape（mesh）中所有面的材质都是一样的，这里都取0
		shape_material = used_materials[shape.mesh.material_ids[0]];
		shape_diff_tex_name = mtlBaseDir + diffuse_texture_name[shape.mesh.material_ids[0]];
		shape_spec_tex_name = mtlBaseDir + specular_texture_name[shape.mesh.material_ids[0]];

		Mesh* new_mesh = new Mesh(vertices, indices, shape_material, shape_diff_tex_name, shape_spec_tex_name);
		meshes.push_back(std::move(*new_mesh));
	}
}

Model::~Model() {
	meshes.clear();
}

void Model::draw(Shader& shader) const {
	for (const auto& mesh : meshes) {
		mesh.draw(shader);
	}
}

