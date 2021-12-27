#include "collision.h"


AABB::AABB() :
	x_min(0.0f),
	x_max(0.0f),
	y_min(0.0f),
	y_max(0.0f),
	z_min(0.0f),
	z_max(0.0f),
	is_empty(true)
	{}


void AABB::add_vertex(glm::vec3 new_vertex)
{
	if (is_empty) {
		x_min = new_vertex.x;
		x_max = new_vertex.x;
		y_min = new_vertex.y;
		y_max = new_vertex.y;
		z_min = new_vertex.z;
		z_max = new_vertex.z;
		is_empty = false;
	}
	else {
		x_min = fmin(new_vertex.x, x_min);
		x_max = fmax(new_vertex.x, x_max);
		y_min = fmin(new_vertex.y, y_min);
		y_max = fmax(new_vertex.y, y_max);
		z_min = fmin(new_vertex.z, z_min);
		z_max = fmax(new_vertex.z, z_max);
	}
	
}


void AABB::update_box(glm::mat4 model_matrix) {
	std::vector<glm::vec4> point(8);
	point[0] = glm::vec4(x_min, y_min, z_min, 1.0f);
	point[1] = glm::vec4(x_min, y_min, z_max, 1.0f);
	point[2] = glm::vec4(x_min, y_max, z_min, 1.0f);
	point[3] = glm::vec4(x_min, y_max, z_max, 1.0f);
	point[4] = glm::vec4(x_max, y_min, z_min, 1.0f);
	point[5] = glm::vec4(x_max, y_min, z_max, 1.0f);
	point[6] = glm::vec4(x_max, y_max, z_min, 1.0f);
	point[7] = glm::vec4(x_max, y_max, z_max, 1.0f);

	// calculate range
	for (auto p = point.begin(); p != point.end(); p++) {
		*p = model_matrix * *p;
	}
	x_range[0] = x_range[1] = point[0].x;
	y_range[0] = y_range[1] = point[0].y;
	z_range[0] = z_range[1] = point[0].z;
	for (auto p = point.begin()+1; p != point.end(); p++) {
		x_range[0] = fmin(x_range[0], p->x);
		x_range[1] = fmax(x_range[1], p->x);
		y_range[0] = fmin(y_range[0], p->y);
		y_range[1] = fmax(y_range[1], p->y);
		z_range[0] = fmin(z_range[0], p->z);
		z_range[1] = fmax(z_range[1], p->z);
	}

	// calculate center
	center.x = (x_range[0] + x_range[1]) / 2.0f;
	center.y = (y_range[0] + y_range[1]) / 2.0f;
	center.z = (z_range[0] + z_range[1]) / 2.0f;

}


glm::vec3 AABB::get_center(void) const 
{
	return center;
}

glm::vec2 AABB::get_x_range(void) const
{
	return x_range;
}

glm::vec2 AABB::get_y_range(void) const
{
	return y_range;
}

glm::vec2 AABB::get_z_range(void) const
{
	return z_range;
}


void AABB::cube_box(float edge) {
	x_min = - edge / 2;
	x_max = edge / 2;
	y_min = - edge / 2;
	y_max = edge / 2;
	z_min = - edge / 2;
	z_max = edge / 2;
}

void AABB::cone_box(float height, float r) {
	x_min = - r;
	x_max = r;
	y_min = - height / 2;
	y_max = height / 2;
	z_min = - r;
	z_max = r;
}

void AABB::sphere_box(float r) {
	x_min = - r;
	x_max = r;
	y_min = - r;
	y_max = r;
	z_min = - r;
	z_max = r;
}

void AABB::cylinder_box(float height, float r) {
	x_min = - r;
	x_max = r;
	y_min = - height / 2;
	y_max = height / 2;
	z_min = - r;
	z_max = r;
}

void AABB::prism_box(float height, float max_edge) {
	x_min = - max_edge / 2;
	x_max = max_edge / 2;
	y_min = - height / 2;
	y_max = height / 2;
	z_min = - max_edge / 2;
	z_max = max_edge / 2;
}

void AABB::square_pyramid_box(float height, float max_edge) {
	x_min = - max_edge / 2;
	x_max = max_edge / 2;
	y_min = - height / 2;
	y_max = height / 2;
	z_min = - max_edge / 2;
	z_max = max_edge / 2;
}

