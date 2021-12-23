#include "world.h"

world::world() {
	this->_windowTitle = std::string("World Rendering");

	// set input mode
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	_mouseInput.move.xOld = _mouseInput.move.xCurrent = 0.5 * _windowWidth;
	_mouseInput.move.yOld = _mouseInput.move.yCurrent = 0.5 * _windowHeight;
	glfwSetCursorPos(_window, _mouseInput.move.xCurrent, _mouseInput.move.yCurrent);

	camera.reset(new PerspectiveCamera(glm::radians(45.0f), 1.0f * _windowWidth / _windowHeight, 0.1f, 10000.0f));
	camera->position = glm::vec3(0.0f, 0.0f, 10.0f);

	skyBox.reset(new SkyBox());	

	nanosuit.reset(new Model("./data/nanosuit_model/nanosuit.obj"));
	nanosuit->position = glm::vec3(0.0f, 0.0f, -20.0f);

	bunny.reset(new Model("./data/bunny_model/bunny.obj"));
	bunny->position = glm::vec3(0.0f, 0.0f, 0.0f);

	cube.reset(new Cube());
	cube->position = glm::vec3(0.0f, 0.0f, -40.0f);

	square_pyramid.reset(new Square_pyramid());
	square_pyramid->position = glm::vec3(0.0f, 0.0f, 20.0f);

	prism.reset(new Prism());
	prism->position = glm::vec3(0.0f, 0.0f, 40.0f);

	sun.reset(new Model("./data/sphere_model/sphere.obj"));
	sunLight.reset(new SunLight(70, 15));
	sunLight->intensity = 0.1f;

	// set shaders
	defaultShader.reset(new Shader(
		std::string("./shader/default_vertex_shader.vert"),
		std::string("./shader/default_frag_shader.frag")
	));

	phongShader.reset(new Shader(
		std::string("./shader/default_phong_vertex_shader.vert"),
		std::string("./shader/default_phong_frag_shader.frag")
	));

	sunShader.reset(new Shader(
		std::string("./shader/sun_vertex_shader.vert"),
		std::string("./shader/sun_frag_shader.frag")
	));

	basicShader.reset(new Shader(
		std::string("./shader/basic_shader.vert"),
		std::string("./shader/basic_shader.frag")
	));
}


void world::renderFrame() {
	// local variables
	glm::mat4 view = camera->getViewMatrix();
	glm::mat4 projection = camera->getProjectionMatrix();
	glm::vec3 eyes = camera->position;
	
	// update title
	showFpsInWindowTitle();
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// clear screen
	glClearColor(_clearColor.r, _clearColor.g, _clearColor.b, _clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// update sun status
	sunLight->updateLight(_deltaTime);
	sun->position = sunLight->getPos();
	sun->scale = glm::vec3(2 * sunLight->radius);

	// draw sun
	sunShader->use();
	sunShader->loadCamera(view, projection);
	sunShader->setVec3("color", sunLight->color);
	sun->Draw(*sunShader);

	// update other shaders
	defaultShader->loadCamera(view, projection);
	phongShader->loadCamera(view, projection);
	phongShader->loadDirectionalLight(*sunLight, eyes);
	basicShader->loadCamera(view, projection);
	basicShader->loadDirectionalLight(*sunLight, eyes);
	
	// draw other models
	bunny->Draw(*phongShader);
	nanosuit->Draw(*phongShader);
	cube->Draw(*basicShader);
	square_pyramid->Draw(*basicShader);
	prism->Draw(*basicShader);

	// TO DO: 不知为何天空盒必须放在最后显示
	skyBox->Draw(projection, view, sunLight->getElevationAngle());
	glDisable(GL_BLEND);
}



void world::handleInput() {
	// TO DO: 我们应当对视角的移动加以限制
	const float cameraMoveSpeed = 0.04f;
	const float cameraRotateSpeed = 0.25f;
	const float deltaAngle = 0.001f;
	const float deltaFovy = 0.001f;

	if (_keyboardInput.keyStates[GLFW_KEY_ESCAPE] != GLFW_RELEASE) {
		glfwSetWindowShouldClose(_window, true);
		return;
	}

	if (_keyboardInput.keyStates[GLFW_KEY_W] != GLFW_RELEASE) {
		camera->position += cameraMoveSpeed * camera->getFront();
	}

	if (_keyboardInput.keyStates[GLFW_KEY_A] != GLFW_RELEASE) {
		camera->position -= cameraMoveSpeed * camera->getRight();
	}

	if (_keyboardInput.keyStates[GLFW_KEY_S] != GLFW_RELEASE) {
		camera->position -= cameraMoveSpeed * camera->getFront();
	}

	if (_keyboardInput.keyStates[GLFW_KEY_D] != GLFW_RELEASE) {
		camera->position += cameraMoveSpeed * camera->getRight();
	}

	// press SPACE to go straight up
	if (_keyboardInput.keyStates[GLFW_KEY_SPACE] != GLFW_RELEASE) {
		camera->position += cameraMoveSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
	}

	// press LEFT SHIFT to go straight down
	if (_keyboardInput.keyStates[GLFW_KEY_LEFT_SHIFT] != GLFW_RELEASE) {
		camera->position -= cameraMoveSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
	}


	//QE实现pan（水平运镜）
	if (_keyboardInput.keyStates[GLFW_KEY_Q] != GLFW_RELEASE) {
		glm::quat temp_rotation = { 1.0f * cos(deltaAngle), 0.0f, 1.0f * sin(deltaAngle), 0.0f };
		camera->rotation = temp_rotation * camera->rotation;
	}

	if (_keyboardInput.keyStates[GLFW_KEY_E] != GLFW_RELEASE) {
		glm::quat temp_rotation = { 1.0f * cos(-deltaAngle), 0.0f, 1.0f * sin(-deltaAngle), 0.0f };
		camera->rotation = temp_rotation * camera->rotation;
	}

	if (_mouseInput.move.xCurrent != _mouseInput.move.xOld) {
		const float deltaX = static_cast<float>(_mouseInput.move.xCurrent - _mouseInput.move.xOld);
		const float angle = -cameraRotateSpeed * _deltaTime * deltaX;
		const glm::vec3 axis = { 0.0f, 1.0f, 0.0f };
		glm::quat temp_rotation = { 1.0f * cos(angle), 0.0f, 1.0f * sin(angle), 0.0f };
		camera->rotation = temp_rotation * camera->rotation;
		_mouseInput.move.xOld = _mouseInput.move.xCurrent;
	}

	if (_mouseInput.move.yCurrent != _mouseInput.move.yOld) {
		const float deltaY = static_cast<float>(_mouseInput.move.yCurrent - _mouseInput.move.yOld);
		const float angle = -cameraRotateSpeed * _deltaTime * deltaY;
		const glm::vec3 axis = camera->getRight();
		glm::quat temp_rotation = { 1.0f * cos(angle), axis[0] * sin(angle), axis[1] * sin(angle), axis[2] * sin(angle) };
		camera->rotation = temp_rotation * camera->rotation;
		_mouseInput.move.yOld = _mouseInput.move.yCurrent;
	}

	// 按R键复原视角
	if (_keyboardInput.keyStates[GLFW_KEY_R] != GLFW_RELEASE) {
		camera.reset(new PerspectiveCamera(glm::radians(45.0f), 1.0f * _windowWidth / _windowHeight, 0.1f, 10000.0f));
		camera->position.z = 10.0f;
		camera->position.y = 0.0f;
	}

	// 按1键进行Zoom In
	if (_keyboardInput.keyStates[GLFW_KEY_1] != GLFW_RELEASE) {
		if (camera->fovy > 0.0174533) {
			camera->fovy -= deltaFovy;
		}
	}

	// 按2键进行Zoom Out
	if (_keyboardInput.keyStates[GLFW_KEY_2] != GLFW_RELEASE) {
		if (camera->fovy < 3.1241393) {
			camera->fovy += deltaFovy;
		}
	}

	//orbit+zoom to fit尚在探索，需要拾取功能



	return;
	
}


