#include "../include/sphere.h"


Sphere::Sphere()
{
	mat.Ka = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	mat.Kd = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	mat.Ks = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	mat.shininess = 10.0f;
	collision.sphere_box(5.0f);
	Init();
}

Sphere::~Sphere() {
	DeInit();
}


bool Sphere::Init(float fRadius,
	unsigned int unLongitudeSlices,
	unsigned int unLatitudeSlices)
{

	m_fRadius = fRadius;
	m_unLongitudeSlices = unLongitudeSlices;
	m_unLatitudeSlices = unLatitudeSlices;

	CalculateSphere();
	CreateBuffers();

	return true;
}

void Sphere::DeInit()
{
	glDeleteVertexArrays(1, &VAO_Sphere);
	glDeleteBuffers(1, &VBO_Sphere);
	glDeleteBuffers(1, &EBO_Sphere);
}

void Sphere::CalculateSphere()
{

	//const unsigned int unSizeOfVertexArray = m_unLatitudeSlices * m_unLongitudeSlices;
	//float* fVertexArrayPtr = new float[unSizeOfVertexArray];

	/*Define all the needed data to create a Sphere*/
	const float fPI = acosf(-1);
	/*Vertex Pos in 3D space*/
	float fX = 0.0f;
	float fY = 0.0f;
	float fZ = 0.0f;
	/*Data needed to calculate the needed indeces to draw 2 triangles in 1 sector of the Sphere*/
	unsigned int unPoint1 = 0;
	unsigned int unPoint2 = 0;
	/*Angles THETA and PHI needed to calculate from degrees to 3D points*/
	float fLongitudeAngleTHETA = 0.0f;
	float fLatitudeAnglePHI = 0.0f;

	/*Calculating the steps needed to decide where and how many
	  vertecies are needed to draw the Sphere*/
	float fLongitudeSteps = 2 * fPI / Sphere::m_unLongitudeSlices;
	float fLatitudeSteps = fPI / Sphere::m_unLatitudeSlices;

	/*Beginning with the Latitude*/
	for (unsigned int i = 0; i <= m_unLatitudeSlices; ++i)
	{
		/*Starting from 90 degrees and going to -90 degrees*/
		fLatitudeAnglePHI = fPI / 2 - i * fLatitudeSteps;
		/*Calculating Z*/
		/* (r * sin(PHI)) */
		fZ = m_fRadius * sinf(fLatitudeAnglePHI);

		/*Going to the Longitude*/
		for (unsigned int j = 0; j <= m_unLongitudeSlices; ++j)
		{
			/*Starting from 0 degrees and going to 360 degrees*/
			fLongitudeAngleTHETA = j * fLongitudeSteps;

			/*Calculating X*/
			/* (r * cos(PHI) * cos(THETA)) */
			fX = m_fRadius * cos(fLatitudeAnglePHI) * cos(fLongitudeAngleTHETA);
			/* (r * cos(PHI) * sin(THETA)) */
			fY = m_fRadius * cos(fLatitudeAnglePHI) * sin(fLongitudeAngleTHETA);

			vecVertexPoints.push_back(fX);
			vecVertexPoints.push_back(fY);
			vecVertexPoints.push_back(fZ);
		}
	}

	/*Calculating Indecies*/
	for (unsigned int i = 0; i < m_unLatitudeSlices; ++i)
	{
		unPoint1 = i * (m_unLongitudeSlices + 1);
		unPoint2 = unPoint1 + (m_unLongitudeSlices + 1);

		for (unsigned int j = 0; j < m_unLongitudeSlices; ++j, ++unPoint1, ++unPoint2)
		{
			if (i != 0)
			{
				vecIndexPoints.push_back(unPoint1);
				vecIndexPoints.push_back(unPoint2);
				vecIndexPoints.push_back(unPoint1 + 1);
			}

			if (i != (m_unLatitudeSlices - 1))
			{
				vecIndexPoints.push_back(unPoint1 + 1);
				vecIndexPoints.push_back(unPoint2);
				vecIndexPoints.push_back(unPoint2 + 1);
			}
		}
	}

}

void Sphere::CreateBuffers()
{
	glGenVertexArrays(1, &VAO_Sphere);
	glGenBuffers(1, &VBO_Sphere);
	glGenBuffers(1, &EBO_Sphere);

	glBindVertexArray(VAO_Sphere);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Sphere);
	glBufferData(GL_ARRAY_BUFFER, (unsigned int)vecVertexPoints.size() * sizeof(float), vecVertexPoints.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Sphere);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (unsigned int)vecIndexPoints.size() * sizeof(unsigned int), vecIndexPoints.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Sphere::Draw(Shader& shader)
{
	// activate shader
	shader.use();
	shader.setVec4("material.ka", mat.Ka);
	shader.setVec4("material.kd", mat.Kd);
	shader.setVec4("material.ks", mat.Ks);
	shader.setFloat("material.shininess", mat.shininess);
	shader.setMat4("model", this->getModelMatrix());

	glBindVertexArray(VAO_Sphere);
	glDrawElements(GL_TRIANGLES, vecIndexPoints.size(), GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, vecIndexPoints.size()/3);
}
