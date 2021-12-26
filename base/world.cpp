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
	nanosuit->rotation = glm::quat(cos(PI/2), glm::vec3(0, 1.0f, 0) * sin(PI/2));
	nanosuit->colli_box.update_box(nanosuit->getModelMatrix());
	colli_box.push_back(nanosuit->colli_box);

	bunny.reset(new Model("./data/bunny_model/bunny.obj"));
	bunny->position = glm::vec3(0.0f, 0.0f, 0.0f);
	bunny->colli_box.update_box(bunny->getModelMatrix());
	colli_box.push_back(bunny->colli_box);

	cube.reset(new Cube());
	cube->position = glm::vec3(0.0f, 0.0f, -40.0f);

	sun.reset(new Model("./data/sphere_model/sphere.obj"));
	sunLight.reset(new SunLight(70, 15));

	//posture.reset(new DynamicModel("./data/postures/pose", 101, 20));
	//posture->setPosition(glm::vec3(0.0f, 0.0f, -60.0f));
	//posture->setScale(glm::vec3(0.1f, 0.1f, 0.1f));

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

	bunnyShader.reset(new Shader(
		std::string("./shader/bunny_shader.vert"),
		std::string("./shader/bunny_shader.frag")
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
	bunnyShader->loadCamera(view, projection);
	bunnyShader->loadDirectionalLight(*sunLight, eyes);

	
	// draw other models
	//posture->Draw(*phongShader, _accumulatedTime);
	bunny->Draw(*bunnyShader);
	nanosuit->Draw(*phongShader);

	// TO DO: 不知为何天空盒必须放在最后显示
	skyBox->Draw(projection, view, sunLight->getElevationAngle());
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
		//camera->position += cameraMoveSpeed * camera->getFront();
		CameraCollisionCheck(camera->position, cameraMoveSpeed * camera->getFront());
	}

	if (_keyboardInput.keyStates[GLFW_KEY_A] != GLFW_RELEASE) {
		//camera->position -= cameraMoveSpeed * camera->getRight();
		CameraCollisionCheck(camera->position, - cameraMoveSpeed * camera->getRight());
	}

	if (_keyboardInput.keyStates[GLFW_KEY_S] != GLFW_RELEASE) {
		//camera->position -= cameraMoveSpeed * camera->getFront();
		CameraCollisionCheck(camera->position, - cameraMoveSpeed * camera->getFront());
	}

	if (_keyboardInput.keyStates[GLFW_KEY_D] != GLFW_RELEASE) {
		//camera->position += cameraMoveSpeed * camera->getRight();
		CameraCollisionCheck(camera->position, cameraMoveSpeed * camera->getRight());
	}

	// press SPACE to go straight up
	if (_keyboardInput.keyStates[GLFW_KEY_SPACE] != GLFW_RELEASE) {
		//camera->position += cameraMoveSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
		CameraCollisionCheck(camera->position, cameraMoveSpeed * glm::vec3(0.0f, 1.0f, 0.0f));
	}

	// press LEFT SHIFT to go straight down
	if (_keyboardInput.keyStates[GLFW_KEY_LEFT_SHIFT] != GLFW_RELEASE) {
		//camera->position -= cameraMoveSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
		CameraCollisionCheck(camera->position, - cameraMoveSpeed * glm::vec3(0.0f, 1.0f, 0.0f));
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


	// 点击左键设定目标
	if (_mouseInput.click.left) {
		int screen_w, screen_h, pixel_w, pixel_h;
		double xpos, ypos, zpos=0;
		glfwGetWindowSize(_window, &screen_w, &screen_h);
		glfwGetFramebufferSize(_window, &pixel_w, &pixel_h);
		glfwGetCursorPos(_window, &xpos, &ypos);
		//glm::vec2 screen_pos = glm::vec2(xpos, ypos);
		glm::vec2 screen_pos = glm::vec2((float)(_windowWidth/2), (float)(_windowHeight/2));
		glm::vec2 pixel_pos = screen_pos * glm::vec2(pixel_w, pixel_h) / glm::vec2(screen_w, screen_h);
		pixel_pos = pixel_pos + glm::vec2(0.5f, 0.5f);
		glm::vec3 win = glm::vec3(pixel_pos.x, pixel_h - 1 - pixel_pos.y, 0.0f);
		glReadPixels(win.x, win.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &win.z);
		glm::vec4 viewport(0.0f, 0.0f, (float)_windowWidth, (float)_windowHeight);
		glm::vec3 from_eyes = glm::unProject(win, camera->getViewMatrix() * camera->getModelMatrix(), camera->getProjectionMatrix(), viewport);
		viewDir = glm::normalize(camera->getRight()) * from_eyes.x + glm::normalize(camera->getUp()) * from_eyes.y - glm::normalize(camera->getFront()) * from_eyes.z;
		// 每一步不能移动过远
		if (glm::length(viewDir) <= 100.0f) {
			target = camera->position + viewDir;
			setTarget = true;
		}
	}

	// 按3键进行Zoom to Fit，使用一次后该目标失效
	if (_keyboardInput.keyStates[GLFW_KEY_3] != GLFW_RELEASE) {
		if (setTarget) {
			camera->position += 0.5f * viewDir;
			setTarget = false;
		}
	}

	// 按4键进行Orbit：根据目标点的z与x坐标发出竖直轴，相机自身位置绕该轴旋转
	if (_keyboardInput.keyStates[GLFW_KEY_4] != GLFW_RELEASE) {
		if (setTarget) {
			//glm::vec2 dxz = glm::mat2x2(cos(deltaAngle), sin(deltaAngle), -sin(deltaAngle), cos(deltaAngle)) * glm::vec2(camera->position.x - target.x, camera->position.z - target.z);
			camera->position.x = cos(deltaAngle) * (camera->position.x - target.x) + sin(deltaAngle) * (camera->position.z - target.z) + target.x;
			camera->position.z = -sin(deltaAngle) * (camera->position.x - target.x) + cos(deltaAngle) * (camera->position.z - target.z) + target.z;
			glm::quat temp_rotation = { 1.0f * cos(0.5f * deltaAngle), 0.0f, 1.0f * sin(0.5f * deltaAngle), 0.0f };
			camera->rotation = temp_rotation * camera->rotation;
		}
	}

	// 按P键截屏
	if (_keyboardInput.keyStates[GLFW_KEY_P] != GLFW_RELEASE) {
		FILE* pDummyFile;  //指向另一bmp文件，用于复制它的文件头和信息头数据
		FILE* pWritingFile;  //指向要保存截图的bmp文件
		GLubyte* pPixelData;    //指向新的空的内存，用于保存截图bmp文件数据
		GLubyte  BMP_Header[BMP_Header_Length];
		GLint    i, j;
		GLint    PixelDataLength;   //BMP文件数据总长度

		// 计算像素数据的实际长度
		i = _windowWidth * 3;   // 得到每一行的像素数据长度
		while (i % 4 != 0)      // 补充数据，直到i是的倍数
			++i;
		PixelDataLength = i * _windowHeight;  //补齐后的总位数

		// 分配内存和打开文件
		pPixelData = (GLubyte*)malloc(PixelDataLength);
		if (pPixelData == 0)
			exit(0);

		fopen_s(&pDummyFile, "bitmapheader.bmp", "rb");

		/*极小BUG待修改
		* 令人费解阿
		* 按理来说要检验一下这个拿来充当文件头的bmp是不是存在
		* 如果不存在就会走入下面这个分支
		* 目前是可以完美运行的 但是又是会走入这个分支的
		* 如果我把下面那个exit的注释去掉
		* 就会直接退出
		*/
		if (pDummyFile == 0);
			//exit(0);

		fopen_s(&pWritingFile, "snapshot.bmp", "wb");

		//把读入的bmp文件的文件头和信息头数据复制，并修改宽高数据
		fread(BMP_Header, sizeof(BMP_Header), 1, pDummyFile);  //读取文件头和信息头，占据54字节
		fwrite(BMP_Header, sizeof(BMP_Header), 1, pWritingFile);
		fseek(pWritingFile, 0x0012, SEEK_SET); //移动到0X0012处，指向图像宽度所在内存
		i = _windowWidth;
		j = _windowHeight;
		fwrite(&i, sizeof(i), 1, pWritingFile);
		fwrite(&j, sizeof(j), 1, pWritingFile);

		// 读取当前画板上图像的像素数据
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);  //设置4位对齐方式
		glReadPixels(0, 0, _windowWidth, _windowHeight, GL_BGR, GL_UNSIGNED_BYTE, pPixelData);

		// 写入像素数据
		fseek(pWritingFile, 0, SEEK_END);
		//把完整的BMP文件数据写入pWritingFile
		fwrite(pPixelData, PixelDataLength, 1, pWritingFile);

		// 释放内存和关闭文件
		fclose(pDummyFile);
		fclose(pWritingFile);
		free(pPixelData);
	}



	return;

}



void world::CameraCollisionCheck(glm::vec3& camera_pos, glm::vec3 move)
{
	glm::vec3 dest = camera_pos + move;
	for (auto ibox = colli_box.begin(); ibox != colli_box.end(); ibox++) {
		bool is_collision = 
			(dest.x > ibox->get_x_range().x) && (dest.x < ibox->get_x_range().y) &&
			(dest.y > ibox->get_y_range().x) && (dest.y < ibox->get_y_range().y) &&
			(dest.z > ibox->get_z_range().x) && (dest.z < ibox->get_z_range().y);
		if (is_collision) {
			cout << "collision" << endl;
			return;
		}
	}
	camera_pos = camera_pos + move;
}
