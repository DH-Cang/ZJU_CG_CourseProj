#include "test.h"

test::test() {
	this->_windowTitle = std::string("test window title");

	camera.reset(new PerspectiveCamera(glm::radians(45.0f), 1.0f * _windowWidth / _windowHeight, 0.1f, 10000.0f));
	camera->position.z = 50.0f;

	

	test_shader.reset(new Shader(
		std::string("./shader/test_vertex_shader.txt"),
		std::string("./shader/test_frag_shader.txt")
		));

	tmp_model.reset(new Model("./data/nanosuit_model/nanosuit.obj"));
}


void test::handleInput() {
	const float cameraMoveSpeed = 0.005f;
	const float cameraRotateSpeed = 0.02f;

	if (_keyboardInput.keyStates[GLFW_KEY_ESCAPE] != GLFW_RELEASE) {
		glfwSetWindowShouldClose(_window, true);
		return;
	}


	if (_keyboardInput.keyStates[GLFW_KEY_W] != GLFW_RELEASE) {
		std::cout << "W" << std::endl;
		camera->position += cameraMoveSpeed * camera->getFront();
	}

	if (_keyboardInput.keyStates[GLFW_KEY_A] != GLFW_RELEASE) {
		std::cout << "A" << std::endl;
		camera->position -= cameraMoveSpeed * camera->getRight();
	}

	if (_keyboardInput.keyStates[GLFW_KEY_S] != GLFW_RELEASE) {
		std::cout << "S" << std::endl;
		camera->position -= cameraMoveSpeed * camera->getFront();
	}

	if (_keyboardInput.keyStates[GLFW_KEY_D] != GLFW_RELEASE) {
		std::cout << "D" << std::endl;
		camera->position += cameraMoveSpeed * camera->getRight();
	}

	if (_mouseInput.move.xCurrent != _mouseInput.move.xOld) {
		std::cout << "mouse move in x direction" << std::endl;
		const float deltaX = static_cast<float>(_mouseInput.move.xCurrent - _mouseInput.move.xOld);
		const float angle = -cameraRotateSpeed * _deltaTime * deltaX;
		const glm::vec3 axis = { 0.0f, 1.0f, 0.0f };
		glm::quat temp_rotation = { 1.0f * cos(angle), 0.0f, 1.0f * sin(angle), 0.0f };
		camera->rotation = temp_rotation * camera->rotation;
		_mouseInput.move.xOld = _mouseInput.move.xCurrent;
	}

	if (_mouseInput.move.yCurrent != _mouseInput.move.yOld) {
		std::cout << "mouse move in y direction" << std::endl;
		const float deltaY = static_cast<float>(_mouseInput.move.yCurrent - _mouseInput.move.yOld);
		const float angle = -cameraRotateSpeed * _deltaTime * deltaY;
		const glm::vec3 axis = camera->getRight();
		glm::quat temp_rotation = { 1.0f * cos(angle), axis[0] * sin(angle), axis[1] * sin(angle), axis[2] * sin(angle) };
		camera->rotation = temp_rotation * camera->rotation;
		_mouseInput.move.yOld = _mouseInput.move.yCurrent;
	}
	return;
}

void test::renderFrame() {
	showFpsInWindowTitle();

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

	tmp_model->Draw(*test_shader);
}

