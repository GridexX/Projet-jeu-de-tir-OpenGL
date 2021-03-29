#ifndef __GLFWEVENTWRAPPER_HPP_
#define __GLFWEVENTWRAPPER_HPP_
#include "demoGLEngine/myapp.hpp"

class GLFWEventWarpper {
public:
	GLFWEventWarpper() = delete;
	GLFWEventWarpper(const GLFWEventWarpper&) = delete;
	GLFWEventWarpper(GLFWEventWarpper&&) = delete;
	~GLFWEventWarpper() = delete;
	static void bind(MyApp* _app);

	static void resizeCallBack(GLFWwindow* window, int width, int height);
	static void onMouseButton(GLFWwindow* window, int button, int action, int mods);
	static void onMouseMove(GLFWwindow* window, double xpos, double ypos);
	static void onMouseScroll(GLFWwindow* window, double xoffset, double yoffset);

private:
	static MyApp* app;
};

#endif