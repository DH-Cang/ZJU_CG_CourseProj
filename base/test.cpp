#include "test.h"
#include "model.h"

test::test() {
	this->_windowTitle = std::string("test window title");

	camera.reset(new PerspectiveCamera(glm::radians(45.0f), 1.0f * _windowWidth / _windowHeight, 0.1f, 10000.0f));
	camera->position.z = 10.0f;

	test_shader.reset(new Shader(
		std::string("./shader/test_vertex_shader.vert"),
		std::string("./shader/test_frag_shader.frag")
		));

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

	tmp_model->draw(*test_shader);
}

