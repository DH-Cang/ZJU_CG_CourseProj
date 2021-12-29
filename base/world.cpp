#include "world.h"

world::world() {
	this->_windowTitle = std::string("World Rendering");

	// set input mode
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	_mouseInput.move.xOld = _mouseInput.move.xCurrent = 0.5 * _windowWidth;
	_mouseInput.move.yOld = _mouseInput.move.yCurrent = 0.5 * _windowHeight;
	glfwSetCursorPos(_window, _mouseInput.move.xCurrent, _mouseInput.move.yCurrent);

	camera.reset(new PerspectiveCamera(glm::radians(45.0f), 1.0f * _windowWidth / _windowHeight, 0.1f, 10000.0f));
	camera->position = glm::vec3(0.0f, 4.0f, 0.0f);

	skyBox.reset(new SkyBox());	

	nanosuit.reset(new Model("./data/nanosuit_model/nanosuit.obj"));
	nanosuit->position = glm::vec3(0.0f, 40.0f, -25.0f);
	nanosuit->rotation = glm::quat(cos(PI/2), glm::vec3(0, 1.0f, 0) * sin(PI/2));
	nanosuit->colli_box.update_box(nanosuit->getModelMatrix());
	colli_box.push_back(nanosuit->colli_box);

	bunny.reset(new Model("./data/bunny_model/bunny.obj"));
	bunny->position = glm::vec3(0.0f, 40.0f, 0.0f);
	bunny->colli_box.update_box(bunny->getModelMatrix());
	colli_box.push_back(bunny->colli_box);

	trophy[0].reset(new Model("./data/trophy_model/trophyexport.obj"));
	trophy[0]->position = glm::vec3(95.0f, 6.0f, 95.0f);
	trophy[0]->scale = glm::vec3(3.0f, 3.0f, 3.0f);
	trophy[0]->colli_box.update_box(trophy[0]->getModelMatrix());
	colli_box.push_back(trophy[0]->colli_box);

	trophy[1].reset(new Model("./data/trophy_model/trophyexport.obj"));
	trophy[1]->position = glm::vec3(-95.0f, 6.0f, 95.0f);
	trophy[1]->scale = glm::vec3(3.0f, 3.0f, 3.0f);
	trophy[1]->colli_box.update_box(trophy[1]->getModelMatrix());
	colli_box.push_back(trophy[1]->colli_box);

	trophy[2].reset(new Model("./data/trophy_model/trophyexport.obj"));
	trophy[2]->position = glm::vec3(95.0f, 6.0f, -95.0f);
	trophy[2]->scale = glm::vec3(3.0f, 3.0f, 3.0f);
	trophy[2]->colli_box.update_box(trophy[2]->getModelMatrix());
	colli_box.push_back(trophy[2]->colli_box);

	trophy[3].reset(new Model("./data/trophy_model/trophyexport.obj"));
	trophy[3]->position = glm::vec3(-95.0f, 6.0f, -95.0f);
	trophy[3]->scale = glm::vec3(3.0f, 3.0f, 3.0f);
	trophy[3]->colli_box.update_box(trophy[3]->getModelMatrix());
	colli_box.push_back(trophy[3]->colli_box);


	cube.reset(new Cube());

	square_pyramid.reset(new Square_pyramid());
	square_pyramid->position = glm::vec3(0.0f, 40.0f, 20.0f);
	square_pyramid->collision.update_box(square_pyramid->getModelMatrix());
	colli_box.push_back(square_pyramid->collision);

	prism.reset(new Prism());
	prism->position = glm::vec3(0.0f, 40.0f, 40.0f);
	prism->collision.update_box(prism->getModelMatrix());
	colli_box.push_back(prism->collision);

	sphere.reset(new Sphere());
	sphere->position = glm::vec3(0.0f, 40.0f, 50.0f);
	sphere->collision.update_box(sphere->getModelMatrix());
	colli_box.push_back(sphere->collision);

	cone.reset(new Cone());
	cone->position = glm::vec3(0.0f, 40.0f, 50.0f);
	cone->collision.update_box(cone->getModelMatrix());
	colli_box.push_back(cone->collision);

	cylinder.reset(new Cylinder());
	cylinder->position = glm::vec3(0.0f, 40.0f, 70.0f);
	cylinder->collision.update_box(cylinder->getModelMatrix());
	colli_box.push_back(cylinder->collision);

	sun.reset(new Model("./data/sphere_model/sphere.obj"));
	sunLight.reset(new SunLight(70, 15));
	sunLight->intensity = 0.1f;
	
	posture.reset(new DynamicModel("./data/postures/pose", 101, 20));
	posture->setPosition(glm::vec3(0.0f, 40.0f, -40.0f));
	posture->setScale(glm::vec3(0.1f, 0.1f, 0.1f));
	
	// set shaders
	nanosuitShader.reset(new Shader(
		std::string("./shader/nanosuit_shader.vert"),
		std::string("./shader/nanosuit_shader.frag")
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

	postureShader.reset(new Shader(
		std::string("./shader/posture_shader.vert"),
		std::string("./shader/posture_shader.frag")
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
	nanosuitShader->loadCamera(view, projection);
	nanosuitShader->loadDirectionalLight(*sunLight, eyes);
	basicShader->loadCamera(view, projection);
	basicShader->loadDirectionalLight(*sunLight, eyes);
	bunnyShader->loadCamera(view, projection);
	bunnyShader->loadDirectionalLight(*sunLight, eyes);
	postureShader->loadCamera(view, projection);

	
	// draw other models
	posture->Draw(*postureShader, _accumulatedTime);
	bunny->Draw(*bunnyShader);
	nanosuit->Draw(*nanosuitShader);
	square_pyramid->Draw(*basicShader);
	prism->Draw(*basicShader);
	sphere->Draw(*basicShader);
	cone->Draw(*basicShader);
	cylinder->Draw(*basicShader);
	trophy[0]->Draw(*basicShader);
	trophy[1]->Draw(*basicShader);
	trophy[2]->Draw(*basicShader);
	trophy[3]->Draw(*basicShader);

	//这里绘制地板(感觉地板可以换成反射系数更加柔和的状态)
	cube->SetKa(glm::vec4(0x99 / 255.0f, 0xCC / 255.0f, 0xCC / 255.0f, 1.0f));
	cube->SetKd(glm::vec4(0x99 / 255.0f, 0xCC / 255.0f, 0xCC / 255.0f, 1.0f));
	cube->scale = glm::vec3(100.0f, 1.0f, 100.0f);
	cube->position = glm::vec3(0.0f, -1.0f, 0.0f);
	cube->Draw(*basicShader);
	if (init_collision_box == false) {
		cube->collision.update_box(cube->getModelMatrix());
		colli_box.push_back(cube->collision);
	}

	cube->SetKa(glm::vec4(0x33 / 255.0f, 0x66 / 255.0f, 0x99 / 255.0f, 1.0f));
	cube->SetKd(glm::vec4(0x33 / 255.0f, 0x66 / 255.0f, 0x99 / 255.0f, 1.0f));
	// 这里开始绘制由立方体搭建的迷宫
	//下放四个语句是迷宫的界
	cube->scale = glm::vec3(100.0f, 10.0f, 1.0f);
	cube->position = glm::vec3(-10.0f, 5.0f, -100.0f);
	cube->Draw(*basicShader); //后方边界
	if (init_collision_box == false) {
		cube->collision.update_box(cube->getModelMatrix());
		colli_box.push_back(cube->collision);
	}

	cube->scale = glm::vec3(100.0f, 10.0f, 1.0f);
	cube->position = glm::vec3(10.0f, 5.0f, 100.0f);
	cube->Draw(*basicShader); //前方边界
	if (init_collision_box == false) {
		cube->collision.update_box(cube->getModelMatrix());
		colli_box.push_back(cube->collision);
	}

	cube->scale = glm::vec3(1.0f, 10.0f, 100.0f);
	cube->position = glm::vec3(-100.0f, 5.0f, 10.0f);
	cube->Draw(*basicShader); //左方边界
	if (init_collision_box == false) {
		cube->collision.update_box(cube->getModelMatrix());
		colli_box.push_back(cube->collision);
	}

	cube->scale = glm::vec3(1.0f, 10.0f, 100.0f);
	cube->position = glm::vec3(100.0f, 5.0f, -10.0f);
	cube->Draw(*basicShader);//右方边界
	if (init_collision_box == false) {
		cube->collision.update_box(cube->getModelMatrix());
		colli_box.push_back(cube->collision);
	}

	//之后是内部的迷宫
	for (int i = 0; i < 100; i += 10)
	{
		cube->scale = glm::vec3(100.0f - i, 4.0f, 1.0f);
		cube->position = glm::vec3(-10.0f, 4.0f, -100.0f + i);
		cube->Draw(*basicShader); //后方矮墙
		if (init_collision_box == false) {
			cube->collision.update_box(cube->getModelMatrix());
			colli_box.push_back(cube->collision);
		}


		cube->scale = glm::vec3(100.0f - i, 4.0f, 1.0f);
		cube->position = glm::vec3(10.0f, 4.0f, 100.0f - i);
		cube->Draw(*basicShader); //前方矮墙
		if (init_collision_box == false) {
			cube->collision.update_box(cube->getModelMatrix());
			colli_box.push_back(cube->collision);
		}

		cube->scale = glm::vec3(1.0f, 4.0f, 100.0f - i);
		cube->position = glm::vec3(-100.0f + i, 4.0f, 10.0f);
		cube->Draw(*basicShader); //左方矮墙
		if (init_collision_box == false) {
			cube->collision.update_box(cube->getModelMatrix());
			colli_box.push_back(cube->collision);
		}

		cube->scale = glm::vec3(1.0f, 4.0f, 100.0f - i);
		cube->position = glm::vec3(100.0f - i, 4.0f, -10.0f);
		cube->Draw(*basicShader); //右方矮墙
		if (init_collision_box == false) {
			cube->collision.update_box(cube->getModelMatrix());
			colli_box.push_back(cube->collision);
		}


	}

	skyBox->Draw(projection, view, sunLight->getElevationAngle());

	init_collision_box = true;
}



void world::handleInput() {
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
		if (state == 0) {
			glm::vec3 direction = camera->getFront();
			direction.y = 0.0f;
			direction = glm::normalize(direction);
			CameraCollisionCheck(camera->position, cameraMoveSpeed * direction);
		}
		else {
			CameraCollisionCheck(camera->position, cameraMoveSpeed * camera->getFront());
		}
	}

	if (_keyboardInput.keyStates[GLFW_KEY_A] != GLFW_RELEASE) {
		//camera->position -= cameraMoveSpeed * camera->getRight();
		if (state == 0) {
			glm::vec3 direction = camera->getRight();
			direction.y = 0.0f;
			direction = glm::normalize(direction);
			CameraCollisionCheck(camera->position, - cameraMoveSpeed * direction);
		}
		else {
			CameraCollisionCheck(camera->position, - cameraMoveSpeed * camera->getRight());
		}
	}

	if (_keyboardInput.keyStates[GLFW_KEY_S] != GLFW_RELEASE) {
		//camera->position -= cameraMoveSpeed * camera->getFront();
		if (state == 0) {
			glm::vec3 direction = camera->getFront();
			direction.y = 0.0f;
			direction = glm::normalize(direction);
			CameraCollisionCheck(camera->position, - cameraMoveSpeed * direction);
		}
		else {
			CameraCollisionCheck(camera->position, - cameraMoveSpeed * camera->getFront());
		}
	}

	if (_keyboardInput.keyStates[GLFW_KEY_D] != GLFW_RELEASE) {
		//camera->position += cameraMoveSpeed * camera->getRight();
		if (state == 0) {
			glm::vec3 direction = camera->getRight();
			direction.y = 0.0f;
			direction = glm::normalize(direction);
			CameraCollisionCheck(camera->position, cameraMoveSpeed * direction);
		}
		else {
			CameraCollisionCheck(camera->position, cameraMoveSpeed * camera->getRight());
		}
	}

	if (_keyboardInput.keyStates[GLFW_KEY_C] == GLFW_PRESS) {
		if (state == 0) {	// game state
			state = 1;
		}
		else {	// god state
			state = 0;
			camera->position = glm::vec3(0.0f, 4.0f, 0.0f);
			camera->rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		}		
		_keyboardInput.keyStates[GLFW_KEY_C] = GLFW_RELEASE;//注意这个语句的作用，是用来除去按键抖动的操作。
	}

	
	
	// press SPACE to go straight up
	if (_keyboardInput.keyStates[GLFW_KEY_SPACE] != GLFW_RELEASE) {
		//camera->position += cameraMoveSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
		if (state) {
			CameraCollisionCheck(camera->position, cameraMoveSpeed * glm::vec3(0.0f, 1.0f, 0.0f));
		}
	}

	// press LEFT SHIFT to go straight down
	if (_keyboardInput.keyStates[GLFW_KEY_LEFT_SHIFT] != GLFW_RELEASE) {
		//camera->position -= cameraMoveSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
		if (state) {
			CameraCollisionCheck(camera->position, -cameraMoveSpeed * glm::vec3(0.0f, 1.0f, 0.0f));
		}
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
		glm::vec3 temp_pos(camera->position);
		glm::quat temp_rotate(camera->rotation);
		camera.reset(new PerspectiveCamera(glm::radians(45.0f), 1.0f * _windowWidth / _windowHeight, 0.1f, 10000.0f));
		camera->position = temp_pos;
		camera->rotation = temp_rotate;
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
			(dest.x > ibox->get_x_range().x - camera->znear) && (dest.x < ibox->get_x_range().y + camera->znear) &&
			(dest.y > ibox->get_y_range().x - camera->znear) && (dest.y < ibox->get_y_range().y + camera->znear) &&
			(dest.z > ibox->get_z_range().x - camera->znear) && (dest.z < ibox->get_z_range().y + camera->znear);
		if (is_collision) {
			cout << "collision" << endl;
			return;
		}
	}
	camera_pos = camera_pos + move;
}
