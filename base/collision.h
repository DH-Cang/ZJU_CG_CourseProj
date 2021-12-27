#pragma once
#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <cmath>
#include <vector>



class AABB {
public:
	AABB();
	AABB(const AABB& src) = default;

	~AABB() = default;

	void add_vertex(glm::vec3 new_vertex);	// add a vertex to expand AABB

	void update_box(glm::mat4 model_matrix);

	glm::vec3 get_center(void) const;
	glm::vec2 get_x_range(void) const;
	glm::vec2 get_y_range(void) const;
	glm::vec2 get_z_range(void) const;

private:
	bool is_empty;
	// local
	float x_min;
	float x_max;
	float y_min;
	float y_max;
	float z_min;
	float z_max;

	// world
	glm::vec3 center;
	glm::vec2 x_range;
	glm::vec2 y_range;
	glm::vec2 z_range;
};

