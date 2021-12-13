#include "test.h"
#include "model.h"

test::test() {
	this->_windowTitle = std::string("test window title");

	camera->position.z = 10.0f;

	test_shader.reset(new Shader(
		std::string("./shader/test_vertex_texture_shader.vert"),
		std::string("./shader/test_frag_texture_shader.frag")
		));
	test_texture.reset(new Texture2D("./data/nanosuit_model/glass_dif.png"));

	tmp_model.reset(new Model("./data/bunny_model/bunny.obj"));
}


void test::handleInput() {
	return;
}

void test::renderFrame() {
	showFpsInWindowTitle();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glClearColor(_clearColor.r, _clearColor.g, _clearColor.b, _clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glm::mat4 projection = camera->getProjectionMatrix();
	glm::mat4 view = camera->getViewMatrix();

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down

	test_shader->use();
	test_shader->setMat4("projection", projection);
	test_shader->setMat4("view", view);
	test_shader->setMat4("model", model);
	//test_shader->setVec3("material.albedo", { 1.0f, 1.0f, 1.0f });
	//test_shader->setFloat("material.ka", 0.0);
	//test_shader->setVec3("material.kd", { 1.0f, 1.0f, 1.0f });
	//test_shader->setVec3("material.ks", { 1.0f, 1.0f, 1.0f });
	//test_shader->setFloat("material.shininess", 3);
	glActiveTexture(GL_TEXTURE0);
	test_texture->bind();
	test_shader->setInt("mapKd", 0);

	//test_shader->setVec3("directionalLight.direction", { 10.0f, 10.0f, 0.0f });
	//test_shader->setFloat("directionalLight.intensity", 1.0f);
	//test_shader->setVec3("directionalLight.color", { 1.0f, 0.0f, 0.0f });

	tmp_model->draw(*test_shader);
}

