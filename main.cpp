#define STB_IMAGE_IMPLEMENTATION 
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "base/world.h"


int main() {
	try {
		srand((unsigned)time(NULL));
		world game;
		game.run();
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	catch (...) {
		std::cerr << "Unknown Error" << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}



