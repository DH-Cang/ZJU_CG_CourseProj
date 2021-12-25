#pragma once

#include <chrono>
#include <string>
#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "input.h"
#include "shader.h"

class Application {
public:
	Application();

	virtual ~Application();

	void run();

protected:
	/* window info */
	GLFWwindow* _window = nullptr;	// window object
	std::string _windowTitle;
	int _windowWidth = 1280;
	int _windowHeight = 720;
	bool _windowReized = false;

	/* timer for fps */
	std::chrono::time_point<std::chrono::high_resolution_clock> _lastTimeStamp;
	float _deltaTime = 0.0f;
	float _accumulatedTime = 0.0f;

	/* input handler */
	KeyboardInput _keyboardInput;
	MouseInput _mouseInput;

	/* clear color */
	glm::vec4 _clearColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	void updateTime();

	/* derived class can override this function to handle input */
	virtual void handleInput() = 0;

	/* derived class can override this function to render a frame */
	virtual void renderFrame() = 0;

	void showFpsInWindowTitle();

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

	static void cursorMovedCallback(GLFWwindow* window, double xPos, double yPos);

	static void mouseClickedCallback(GLFWwindow* window, int button, int action, int mods);

	static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

	static void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};