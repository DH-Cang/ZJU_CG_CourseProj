#include "objmodel.h"
using namespace std;

ObjModel::ObjModel(string const& path) {
	ifstream file;
	file.open(path, ios::in);
	if (file.is_open() == false) {
		throw std::runtime_error("fail to open obj file\n");
	}

	char buffer[200];
	vector<glm::vec3> pos;
	vector<glm::vec3> nor;
	while (!file.eof()) {
		file.getline(buffer, 200);

		if (buffer[0] == 'v' && buffer[1] == ' ')
		{
			glm::vec3 t;
			sscanf_s(buffer, "v %f %f %f", &(t.x), &(t.y), &(t.z));
			pos.push_back(t);
		}
		else if (buffer[0] == 'v' && buffer[1] == 'n' && buffer[2] == ' ')
		{
			glm::vec3 n;
			sscanf_s(buffer, "vn %f %f %f", &(n.x), &(n.y), &(n.z));
			nor.push_back(n);
		}
		else if (buffer[0] == 'f' && buffer[1] == ' ')
		{
			unsigned int p1, n1, p2, n2, p3, n3;
			struct ver temp;
			sscanf_s(buffer, "f %u//%u %u//%u %u//%u", &p1, &n1, &p2, &n2, &p3, &n3);

			temp.pos = pos[p1-1];
			temp.normal = nor[n1-1];
			vertices.push_back(temp);
			temp.pos = pos[p2-1];
			temp.normal = nor[n2-1];
			vertices.push_back(temp);
			temp.pos = pos[p3-1];
			temp.normal = nor[n3-1];
			vertices.push_back(temp);
		}
	}

	initGL();
	return;
}


void ObjModel::initGL(void) {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(struct ver), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void ObjModel::Draw(Shader& shader)
{
	glDepthFunc(GL_LESS);
	shader.use();
	shader.setVec4("material.ka", glm::vec4(0.3, 0.3, 0.3, 1.0));
	shader.setVec4("material.kd", glm::vec4(0.6, 0.6, 0.6, 1.0));
	shader.setVec4("material.ks", glm::vec4(0.5, 0.5, 0.5, 1.0));
	shader.setFloat("material.shininess", 32);
	shader.setMat4("model", getModelMatrix());

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glBindVertexArray(0);
}

ObjModel::~ObjModel() {
	if (vbo != 0) {
		glDeleteBuffers(1, &vbo);
		vbo = 0;
	}

	if (vao != 0) {
		glDeleteVertexArrays(1, &vao);
		vao = 0;
	}
}
