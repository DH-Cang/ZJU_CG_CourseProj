#pragma once
#include <iostream>
#include <vector>
#include <glad/glad.h>>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../base/shader.h"
#include "../../base/object3d.h"
#include "../../base/material.h"


class Sphere : public Object3D
{

private:

	struct Material mat;

	/*OpenGL Data*/
	unsigned int VBO_Sphere = 0;
	unsigned int VAO_Sphere = 0;
	unsigned int EBO_Sphere = 0;

	/*Radius of Sphere*/
	float m_fRadius = 0.0f;
	/*Longitude Slices*/
	unsigned int m_unLongitudeSlices = 0;
	/*Latitude Slices*/
	unsigned int m_unLatitudeSlices = 0;

	//TODO
	std::vector<float> vecVertexPoints;
	std::vector<unsigned int> vecIndexPoints;

public:

	Sphere();
	~Sphere();
	/*Initialization to all the needed data*/
	bool Init(float fRadius = 5.0f,
		unsigned int unLongitudeSlices = 50,
		unsigned int unLatitudeSlices = 50);

	/*Deinitialize all members*/
	void DeInit();

	/*Function that calculates Vertecies and Indecies
	for a given LongSlices, LatSlices and Radius*/
	void CalculateSphere();

	/*Passing and binding all the given calculated data to the GL Buffers*/
	void CreateBuffers();

	/*Draw the Sphere*/
	void Draw(Shader& shaderSphere);

	inline void SetKa(glm::vec4 ka) { mat.Ka = ka; }
	inline void SetKd(glm::vec4 kd) { mat.Kd = kd; }
	inline void SetKs(glm::vec4 ks) { mat.Ks = ks; }
	inline void SetShiniess(float shininess) { mat.shininess = shininess; }
};
