#include <cstdlib>
#include <iostream>
#include <string>
#include <stdexcept>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "demoGLEngine/myapp.hpp"
#include "demoGLEngine/glfwEventWrapper.hpp"



int main(int argc, char* argv[]) {
	MyApp app(800, 600, "OpenGL Demo");
	app.loop();
	
	return EXIT_SUCCESS;
}
