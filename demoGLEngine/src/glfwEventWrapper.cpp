#include "demoGLEngine/glfwEventWrapper.hpp"

MyApp* GLFWEventWarpper::app = nullptr;

void GLFWEventWarpper::bind(MyApp* _app) {
	GLFWEventWarpper::app = _app;
}

void GLFWEventWarpper::resizeCallBack(GLFWwindow* window, int width, int height) {
	if (app)
		app->onResize(width, height);
}

void GLFWEventWarpper::onMouseButton(GLFWwindow* window, int button, int action, int mods) {
	if (app)
		app->onMouseButton(button, action, mods);
}

void GLFWEventWarpper::onMouseMove(GLFWwindow* window, double xpos, double ypos) {
	if (app)
		app->onMouseMove(xpos, ypos);
}

void GLFWEventWarpper::onMouseScroll(GLFWwindow* window, double xoffset, double yoffset) {
	if (app)
		app->onMouseScroll(xoffset, yoffset);
}
