#include "world.h"

world::world() {
	this->_windowTitle = std::string("World Rendering");

	camera.reset(new PerspectiveCamera(glm::radians(45.0f), 1.0f * _windowWidth / _windowHeight, 0.1f, FARTHEST));
	camera->position.z = 10.0f;
	camera->position.y = 0.0f;

	skyBox.reset(new SkyBox());	

	sunLight.reset(new SunLight(30, -23.5));

	defaultShader.reset(new Shader(
		std::string("./shader/default_vertex_shader.vert"),
		std::string("./shader/default_frag_shader.frag")
	));

	sunShader.reset(new Shader(
		std::string("./shader/default_vertex_shader.vert"),
		std::string("./shader/default_frag_shader.frag")
	));

	nanosuit.reset(new Model("./data/nanosuit_model/nanosuit.obj"));

	bunny.reset(new Model("./data/bunny_model/bunny.obj"));

	sun.reset(new Model("./data/sphere_model/sphere.obj"));


}


void world::handleInput() {

	// TO DO: 我们应当对视角的移动加以限制
	const float cameraMoveSpeed = 0.04f;
	const float cameraRotateSpeed = 0.25f;
	const float deltaAngle = 0.001f;

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

	if (_keyboardInput.keyStates[GLFW_KEY_Q] != GLFW_RELEASE) {
		std::cout << "Q" << std::endl;
		glm::quat temp_rotation = { 1.0f * cos(deltaAngle), 0.0f, 1.0f * sin(deltaAngle), 0.0f };
		camera->rotation = temp_rotation * camera->rotation;
	}

	if (_keyboardInput.keyStates[GLFW_KEY_E] != GLFW_RELEASE) {
		std::cout << "E" << std::endl;
		glm::quat temp_rotation = { 1.0f * cos(-deltaAngle), 0.0f, 1.0f * sin(-deltaAngle), 0.0f };
		camera->rotation = temp_rotation * camera->rotation;
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

	// 按R键复原视角
	if (_keyboardInput.keyStates[GLFW_KEY_R] != GLFW_RELEASE) {
		std::cout << "R" << std::endl;
		camera.reset(new PerspectiveCamera(glm::radians(45.0f), 1.0f * _windowWidth / _windowHeight, 0.1f, FARTHEST));
		camera->position.z = 10.0f;
		camera->position.y = 0.0f;
	}

	return;
}

void world::renderFrame() {

	showFpsInWindowTitle();
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glClearColor(_clearColor.r, _clearColor.g, _clearColor.b, _clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	sunLight->updateLight(_deltaTime);


	glm::mat4 projection = camera->getProjectionMatrix();
	glm::mat4 view = camera->getViewMatrix();

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down

	defaultShader->use();
	defaultShader->setMat4("projection", projection);
	defaultShader->setMat4("view", view);


	bunny->Draw(*defaultShader, { 10.0f, 10.0f, 10.0f });
	nanosuit->Draw(*defaultShader);
	bunny->Draw(*defaultShader, { 20.0f, 10.0f, 10.0f });

	sunShader->use();
	sunShader->setMat4("projection", projection);
	sunShader->setMat4("view", view);
	sun->Draw(*sunShader, sunLight->getPos(), 500.0f);
	


	
	// TO DO: 不知为何天空盒必须放在最后显示
	skyBox->Draw(projection, view, sunLight->getElevationAngle());
}

