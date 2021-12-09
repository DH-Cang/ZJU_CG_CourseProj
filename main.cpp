#define STB_IMAGE_IMPLEMENTATION 
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "base/test.h"


int main() {
	try {
		test test;
		test.run();
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



