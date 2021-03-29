#ifndef __MYAPP_HPP_
#define __MYAPP_HPP_
#include <string>
#include <iostream>

//Raffin objetOff


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glengine/camera.hpp"
#include "glengine/shader.hpp"
#include "glengine/texture.hpp"
#include "glengine/glmesh.hpp"
#include "particle.hpp"
#include "projectileSystem.hpp"
#include "objetsOFF.h"
#include "target.hpp"
#include <unordered_map>


class MyApp {
public:
	MyApp(int _width, int _height, const char* _title);
	~MyApp();

	void onMouseButton(int button, int action, int mods);
	void onMouseMove(double xpos, double ypos);
	void onMouseScroll(double xoffset, double yoffset);
	void onResize(int _width, int _height);
	
	void loop();

private:
	enum class DisplayStyle { normal, depthBuffer, Foggy, nightVision, blur, laplacien, emboss };

	void update(const double& dt);
	void frameBufferConfiguration();
	void render();
	void renderScene();

	void processInput();
	void buildGeometry();
	void initGLFWCallBack();
	
	void testCollision(glm::vec3* cubePositions, float* cubeRotation, unsigned int nbCubes);

	int width;
	int height;
	const char* title;
	GLFWwindow* window;

	static constexpr double MS_PER_UPDATE = 33.0*(1.0/1000.0);  //60 images par seconde (33ms entre chaque frame)
	double deltaTime;
	double lastFrame;
	float lastX;
	float lastY;
	bool leftButtonPressed;
	bool leftButtonReleased;
	bool firstMouse;
	DisplayStyle displayState;
	ParticleSystem* particuleSys;
	ProjectileSystem* projectileSys;
	Target* target;

	Camera* camera;	
	std::unordered_map<std::string, Texture> textures;
	std::unordered_map<std::string, Shader> shaders;
	std::unordered_map<std::string, glMesh> meshes;
	GLuint framebuffer, textureColorbuffer, rbo;
};

#endif // !__MYAPP_HPP_
