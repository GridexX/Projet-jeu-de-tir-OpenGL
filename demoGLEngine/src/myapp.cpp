#include "demoGLEngine/myapp.hpp"
#include "demoGLEngine/glfwEventWrapper.hpp"
#include "glengine/shader.hpp"
#include "demoGLEngine/config.hpp"
#include <stdexcept>
#include <vector>

// #define STB_IMAGE_IMPLEMENTATION
#include "stbimage/stb_image.h"


MyApp::MyApp(int _width, int _height, const char* _title)
	:width(_width), height(_height), title(_title), window(nullptr), framebuffer(0), textureColorbuffer(0), rbo(0),
	camera(nullptr), projectileSys(nullptr), particuleSys(nullptr), target(nullptr),
	deltaTime(0.0f), lastFrame(0.0f), firstMouse(true),
	lastX(0.0f), lastY(0.0f), leftButtonPressed(false), leftButtonReleased(false),
	displayState(DisplayStyle::normal) {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL) {		
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window");
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw std::runtime_error("Failed to initialize GLAD");
	}
	projectileSys = new ProjectileSystem();

	//Loading de la target
	target = new Target(std::string(_resources_directory).append("models/100x100pointsUV.off").c_str());
}

void MyApp::initGLFWCallBack() {
	GLFWEventWarpper::bind(this);
	glfwSetFramebufferSizeCallback(window, GLFWEventWarpper::resizeCallBack);

	glfwSetMouseButtonCallback(window, GLFWEventWarpper::onMouseButton);
	glfwSetCursorPosCallback(window, GLFWEventWarpper::onMouseMove);
	glfwSetScrollCallback(window, GLFWEventWarpper::onMouseScroll);

	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void MyApp::onMouseButton(int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		leftButtonPressed = true;
		leftButtonReleased = false;

	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		leftButtonPressed = false;
		leftButtonReleased = true;
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		projectileSys->addProjectiles(camera->Position + camera->Front, camera->Front);

	}
}

void MyApp::onMouseMove(double xpos, double ypos) {
	if (leftButtonReleased) {
		lastX = (float)xpos;
		lastY = (float)ypos;
	}
	
	if (leftButtonPressed) {
		if (firstMouse) {
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = (float)xpos - lastX;
		float yoffset = lastY - (float)ypos; // reversed since y-coordinates go from bottom to top

		lastX = (float)xpos;
		lastY = (float)ypos;

		camera->ProcessMouseMovement(xoffset, yoffset, true);
	}	
}

void MyApp::onMouseScroll(double xoffset, double yoffset) {
	camera->ProcessMouseScroll((float)yoffset);
}

void MyApp::processInput() {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
		displayState = DisplayStyle::depthBuffer;
	}

	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) {
		displayState = DisplayStyle::normal;
	}

	if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS) {
		displayState = DisplayStyle::Foggy;
	}

	if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS) {
		displayState = DisplayStyle::nightVision;
	}

	if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS) {
		displayState = DisplayStyle::blur;
	}

	if (glfwGetKey(window, GLFW_KEY_F6) == GLFW_PRESS) {
		displayState = DisplayStyle::laplacien;
	}

	if (glfwGetKey(window, GLFW_KEY_F7) == GLFW_PRESS) {
		displayState = DisplayStyle::emboss;
	}

	if (camera) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera->ProcessKeyboard(Camera::Movement::FORWARD, deltaTime);

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera->ProcessKeyboard(Camera::Movement::BACKWARD, deltaTime);

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera->ProcessKeyboard(Camera::Movement::LEFT, deltaTime);

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera->ProcessKeyboard(Camera::Movement::RIGHT, deltaTime);

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			camera->ProcessKeyboard(Camera::Movement::UP, deltaTime);

		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			camera->ProcessKeyboard(Camera::Movement::DOWN, deltaTime);

		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			camera->ProcessKeyboard(Camera::Movement::SPRINT, deltaTime);

		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
			camera->ProcessKeyboard(Camera::Movement::DESPRINT, deltaTime);
	}	
}


void MyApp::buildGeometry() {
	shaders["boxShader"] = Shader(
		std::string(_resources_directory).append("shaders/lighting.vert").c_str(),
		std::string(_resources_directory).append("shaders/lighting.frag").c_str()
	);	

	shaders["normalShader"] = Shader(
		std::string(_resources_directory).append("shaders/normalMapping.vert").c_str(),
		std::string(_resources_directory).append("shaders/normalMapping.frag").c_str()
	);

	shaders["particleShader"] = Shader(
		std::string(_resources_directory).append("shaders/particle.vert").c_str(),
		std::string(_resources_directory).append("shaders/particle.frag").c_str()
	);

	shaders["projectileShader"] = Shader(
		std::string(_resources_directory).append("shaders/projectile.vert").c_str(),
		std::string(_resources_directory).append("shaders/projectile.frag").c_str()
	);

	shaders["lightProxyShader"] = Shader(
		std::string(_resources_directory).append("shaders/lightCube.vert").c_str(),
		std::string(_resources_directory).append("shaders/lightCube.frag").c_str()
	);

	shaders["targetShader"] = Shader(
		std::string(_resources_directory).append("shaders/targetShader.vert").c_str(),
		std::string(_resources_directory).append("shaders/targetShader.frag").c_str()
	);
 



	std::vector<float> cubeVertices = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f, // 0  0
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f, // 1  1
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f, // 2  2
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f, // 3  3

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f, // 0  4
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f, // 1  5
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f, // 2  6
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f, // 3  7

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f, // 0  8
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // 1  9
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f, // 2 10
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f, // 3 11

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f, // 0 12
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // 1 13
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, // 2 14
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f, // 3 15

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f, // 0 16
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f, // 1 17
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f, // 2 18
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f, // 3 19

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f, // 0 20
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f, // 1 21
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, // 2 22
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f  // 3 23
	};

	std::vector<unsigned int> cubeIndices = {
		 0,  1,  2,
		 2,  3,  0,

		 4,  5, 6,
		 6,  7, 4,

		 8,  9, 10,
		10, 11,  8,

		12, 13, 14,
		14, 15,  12,

		16, 17, 18,
		18, 19, 16,

		20, 21, 22,
		22, 23, 20
	};

	//meshes["box"] = glMesh(cubeVertices, cubeIndices, GL_STATIC_DRAW);

	std::vector<glm::vec3> cVertices = {
		// positions
		glm::vec3(-0.5f, -0.5f, -0.5f),
		glm::vec3( 0.5f, -0.5f, -0.5f),
		glm::vec3( 0.5f,  0.5f, -0.5f),
		glm::vec3(-0.5f,  0.5f, -0.5f),
									  
		glm::vec3(-0.5f, -0.5f,  0.5f),
		glm::vec3( 0.5f, -0.5f,  0.5f),
		glm::vec3( 0.5f,  0.5f,  0.5f),
		glm::vec3(-0.5f,  0.5f,  0.5f),
									  
		glm::vec3(-0.5f,  0.5f,  0.5f),
		glm::vec3(-0.5f,  0.5f, -0.5f),
		glm::vec3(-0.5f, -0.5f, -0.5f),
		glm::vec3(-0.5f, -0.5f,  0.5f),
									  
		glm::vec3( 0.5f,  0.5f,  0.5f),
		glm::vec3( 0.5f,  0.5f, -0.5f),
		glm::vec3( 0.5f, -0.5f, -0.5f),
		glm::vec3( 0.5f, -0.5f,  0.5f),
									  
		glm::vec3(-0.5f, -0.5f, -0.5f),
		glm::vec3( 0.5f, -0.5f, -0.5f),
		glm::vec3( 0.5f, -0.5f,  0.5f),
		glm::vec3(-0.5f, -0.5f,  0.5f),
									  
		glm::vec3(-0.5f,  0.5f, -0.5f),
		glm::vec3( 0.5f,  0.5f, -0.5f),
		glm::vec3( 0.5f,  0.5f,  0.5f),
		glm::vec3(-0.5f,  0.5f,  0.5f)
	};

	std::vector<glm::vec3> cNormals = {
		glm::vec3(0.0f,  0.0f, -1.0f),
		glm::vec3(0.0f,  0.0f, -1.0f),
		glm::vec3(0.0f,  0.0f, -1.0f),
		glm::vec3(0.0f,  0.0f, -1.0f),

		glm::vec3(0.0f,  0.0f, 1.0f ),
		glm::vec3(0.0f,  0.0f, 1.0f ),
		glm::vec3(0.0f,  0.0f, 1.0f ),
		glm::vec3(0.0f,  0.0f, 1.0f ), 

		glm::vec3(-1.0f,  0.0f,  0.0f),
		glm::vec3(-1.0f,  0.0f,  0.0f),
		glm::vec3(-1.0f,  0.0f,  0.0f),
		glm::vec3(-1.0f,  0.0f,  0.0f),

		glm::vec3(1.0f,  0.0f,  0.0f),
		glm::vec3(1.0f,  0.0f,  0.0f),
		glm::vec3(1.0f,  0.0f,  0.0f),
		glm::vec3(1.0f,  0.0f,  0.0f),

		glm::vec3(0.0f, -1.0f,  0.0f),
		glm::vec3(0.0f, -1.0f,  0.0f),
		glm::vec3(0.0f, -1.0f,  0.0f),
		glm::vec3(0.0f, -1.0f,  0.0f), 

		glm::vec3(0.0f,  1.0f,  0.0f),
		glm::vec3(0.0f,  1.0f,  0.0f),
		glm::vec3(0.0f,  1.0f,  0.0f),
		glm::vec3(0.0f,  1.0f,  0.0f)
	};

	std::vector<glm::vec2> cUVs = {
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),

		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),

		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),

		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),

		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, 0.0f),

		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, 0.0f)
	};
	
	std::vector<unsigned int> cIndices = {
		 0,  1,  2,
		 2,  3,  0,

		 4,  5, 6,
		 6,  7, 4,

		 8,  9, 10,
		10, 11,  8,

		12, 13, 14,
		14, 15,  12,

		16, 17, 18,
		18, 19, 16,

		20, 21, 22,
		22, 23, 20
	};

	std::vector<glm::vec3> cTangents;
	std::vector<glm::vec3> cBitangents;

	//Mesh des cubes avec les tangentes et les bitanges pour faire la normal map dans le shader
	meshes["box"] = glMesh(cVertices, cNormals, cUVs, cIndices, cTangents, cBitangents, GL_STATIC_DRAW);
	meshes["box"].computeTangentBasis(cVertices, cUVs, cNormals, cTangents, cBitangents);
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	std::vector<glm::vec3> indexed_tangents;
	std::vector<glm::vec3> indexed_bitangents;

	//meshes["box"].indexVBO_TBN(
	//	cVertices, cUVs, cNormals, cTangents, cBitangents,
	//	indices, indexed_vertices, indexed_uvs, indexed_normals, indexed_tangents, indexed_bitangents
	//);
	//meshes["box"] = glMesh(indexed_vertices, indexed_normals, indexed_uvs, indices, indexed_tangents, indexed_bitangents, GL_STATIC_DRAW);

	// load and create a texture 
	// -------------------------
	textures["textureDiffuse"] = Texture(std::string(_resources_directory).append("textures/container2_gris.png").c_str());
	textures["textureSpecular"] = Texture(std::string(_resources_directory).append("textures/container2_specular2.png").c_str());
	textures["textureMask"] = Texture(std::string(_resources_directory).append("textures/container2_diffuse_mask.png").c_str());
	textures["textureParticle"] = Texture(std::string(_resources_directory).append("textures/explosion_sprite.png").c_str());
	textures["textureTarget"] = Texture(std::string(_resources_directory).append("textures/target.png").c_str());
	textures["textureTarget2"] = Texture(std::string(_resources_directory).append("textures/targetLogo.png").c_str());
	//textures["textureNormal"] = Texture(std::string(_resources_directory).append("textures/container_normal.png").c_str());
	
	// shader configuration
    // --------------------
	const Shader &boxShader = shaders["boxShader"];
	boxShader.bind();
	boxShader.setInt("textureDiffuse", 0);
	boxShader.setInt("textureSpecular", 1);
	boxShader.setInt("textureMask", 2);



	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	meshes["lightProxy"] = glMesh(cubeVertices, cubeIndices, GL_STATIC_DRAW);

	// Plan
	std::vector<float> planVertices = {
		// positions			// Normal			// texture coords
		 0.5f,  0.f,   0.5f,	0.0f, 1.0f, 0.0f,	80.0f, 80.0f,
		 0.5f,  0.0f, -0.5f,	0.0f, 1.0f, 0.0f,	0.0f, 80.0f,
		-0.5f,  0.0f, -0.5f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f,
		-0.5f,  0.0f,  0.5f,	0.0f, 1.0f, 0.0f,	80.0f, 0.0f
	};

	std::vector<unsigned int> planIndices = {
		0, 1, 3,   // premier triangle
		1, 2, 3    // second triangle
	};

	meshes["ground"] = glMesh(planVertices, planIndices, GL_STATIC_DRAW);
	textures["ground"] = Texture(std::string(_resources_directory).append("textures/wall.jpg").c_str());

	shaders["groundShader"] = Shader(
		std::string(_resources_directory).append("shaders/planShader.vert").c_str(),
		std::string(_resources_directory).append("shaders/planShader.frag").c_str()
	);

	target->loadBuffer();

}


void MyApp::loop() {
	
	initGLFWCallBack();

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);
	// glViewport(0, 0, width, height);
	camera = new Camera(glm::vec3(0.0f, 2.1f, 12.0f));
	buildGeometry();
	particuleSys = new ParticleSystem(glm::vec3(-2.0f, 0, 3.0f));
	particuleSys->init();
	frameBufferConfiguration();

	double lag = 0.0;
	while (!glfwWindowShouldClose(window)) {
		// per-frame time logic
		// --------------------
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		lag += deltaTime;
		while (lag >= MS_PER_UPDATE) {
			update(MS_PER_UPDATE);
			lag -= MS_PER_UPDATE;
		}

		// input
		processInput();

		// render
		render();

		// check and call events and swap the buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
}



void MyApp::update(const double& dt) {  //simulation des particules et du système de projectiles
	particuleSys->simulate(dt, camera->Position);
	projectileSys->simulate(dt);
}

void MyApp::frameBufferConfiguration() {
	// Clean previous states
	if (framebuffer)
		glDeleteFramebuffers(1, &framebuffer);

	if (textureColorbuffer)
		glDeleteTextures(1, &textureColorbuffer);

	if (rbo)
		glDeleteTextures(1, &rbo);



	std::vector<float> quadVertices = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions 
		-1.0f, -1.0f, 0.0f, // 0
		 1.0f, -1.0f, 0.0f, // 1
		 1.0f,  1.0f, 0.0f, // 2
	   -1.0f,  1.0f, 0.0f  // 3
	};

	std::vector<float> quadUV = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// texCoords		
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	std::vector<unsigned int> quadIndices = {
		0, 1, 2,
		0, 2, 3
	};

	const glMesh& screenQuad = meshes["screenQuad"] = glMesh(quadVertices, {}, quadUV, quadIndices);
	screenQuad.bind();


	// framebuffer configuration
	// -------------------------
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	
	// create a color attachment texture
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
		
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	/*GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo)
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it  */

	glGenTextures(1, &rbo);
	glBindTexture(GL_TEXTURE_2D, rbo);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, rbo, 0);

	// Set the list of draw buffers.
	GLenum DrawBuffers[2] = { GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT };
	glDrawBuffers(2, DrawBuffers); // "1" is the size of DrawBuffers

	// glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, textureColorbuffer);
	// glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw std::runtime_error("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	shaders["colorScreenShader"] = Shader(
		std::string(_resources_directory).append("shaders/framebuffersScreen.vert").c_str(),
		std::string(_resources_directory).append("shaders/colorFramebuffersScreen.frag").c_str()
	);

	shaders["foggyColorScreenShader"] = Shader(
		std::string(_resources_directory).append("shaders/framebuffersScreen.vert").c_str(),
		std::string(_resources_directory).append("shaders/foggyColorScreenShader.frag").c_str()
	);

	shaders["depthScreenShader"] = Shader(
		std::string(_resources_directory).append("shaders/framebuffersScreen.vert").c_str(),
		std::string(_resources_directory).append("shaders/depthFramebuffersScreen.frag").c_str()
	);	

	shaders["nightVision"] = Shader(
		std::string(_resources_directory).append("shaders/framebuffersScreen.vert").c_str(),
		std::string(_resources_directory).append("shaders/nightVision.frag").c_str()
	);

	shaders["blur"] = Shader(
		std::string(_resources_directory).append("shaders/framebuffersScreen.vert").c_str(),
		std::string(_resources_directory).append("shaders/blur.frag").c_str()
	);

	shaders["laplacien"] = Shader(
		std::string(_resources_directory).append("shaders/framebuffersScreen.vert").c_str(),
		std::string(_resources_directory).append("shaders/laplacien.frag").c_str()
	);

	shaders["emboss"] = Shader(
		std::string(_resources_directory).append("shaders/framebuffersScreen.vert").c_str(),
		std::string(_resources_directory).append("shaders/emboss.frag").c_str()
	);

	const Shader& foggyColorScreenShader = shaders["foggyColorScreenShader"];
	foggyColorScreenShader.bind();
	foggyColorScreenShader.setInt("screenTexture", 4);
	foggyColorScreenShader.setInt("screenDepth", 5);
	foggyColorScreenShader.setFloat("viewPort.near", 0.1f);
	foggyColorScreenShader.setFloat("viewPort.far", 100.0f);
	foggyColorScreenShader.setVec3("fog.color", 0.4f, 0.3f, 0.05f);
	foggyColorScreenShader.setFloat("fog.density", 0.08f);

	const Shader& colorScreenShader = shaders["colorScreenShader"];
	colorScreenShader.bind();
	colorScreenShader.setInt("screenTexture", 4);
	colorScreenShader.setInt("screenDepth", 5);
	colorScreenShader.setFloat("viewPort.near", 0.1f);
	colorScreenShader.setFloat("viewPort.far", 100.0f);

	const Shader& depthScreenShader = shaders["depthScreenShader"];
	depthScreenShader.bind();
	depthScreenShader.setInt("screenTexture", 4);
	depthScreenShader.setInt("screenDepth", 5);
	depthScreenShader.setFloat("viewPort.near", 0.1f);
	depthScreenShader.setFloat("viewPort.far", 100.0f);
	//Données du shader pour la nightvision
	const Shader& nightVision = shaders["nightVision"];
	nightVision.bind();
	nightVision.setInt("screenTexture", 4);
	nightVision.setInt("screenDepth", 5);
	nightVision.setFloat("viewPort.near", 0.1f);
	nightVision.setFloat("viewPort.far", 100.0f);
	//Données du shader pour le blur
	const Shader& blur = shaders["blur"];
	blur.bind();
	blur.setInt("screenTexture", 4);


	const Shader& emboss = shaders["emboss"];
	emboss.bind();
	emboss.setInt("screenTexture", 4);


	const Shader& laplacien = shaders["laplacien"];
	laplacien.bind();
	laplacien.setInt("screenTexture", 4);




}

void MyApp::render() {
	// bind to framebuffer and draw scene as we normally would to color texture 
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

	renderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
	
	// clear all relevant buffers
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
	glClear(GL_COLOR_BUFFER_BIT);

	
	switch (displayState) {
	case DisplayStyle::normal:
		shaders["colorScreenShader"].bind();
		break;
	case DisplayStyle::depthBuffer:
		shaders["depthScreenShader"].bind();
		break;
	case DisplayStyle::Foggy:
		shaders["foggyColorScreenShader"].bind();
		break;
	case DisplayStyle::nightVision:
		shaders["nightVision"].bind();
		break;
	case DisplayStyle::blur:
		shaders["blur"].bind();
		break;
	case DisplayStyle::laplacien:
		shaders["laplacien"].bind();
		break;
	case DisplayStyle::emboss:
		shaders["emboss"].bind();
		break;
	default:
		shaders["colorScreenShader"].bind();
		break;
	}
	
	const glMesh& screenQuad = meshes["screenQuad"];
	screenQuad.bind();
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
	
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, rbo);
	screenQuad.render(); 
}

//Fonction des tests des collisions des sphères avec les cubes, le plan et la cible
void MyApp::testCollision(glm::vec3* cubePositions, float* cubeRotation, unsigned int nbCubes)
{
	for (size_t i = 0; i < projectileSys->projectiles.size(); ++i)
	{
		Sphere& s = projectileSys->projectiles.at(i);

		if (s.pos.y < projectileSys->rayon)  //Colision avec le plan( située en y=0 )
		{
			s.life = 0.0f;
			return;
		}
		for (size_t j = 0; j < nbCubes; ++j)  //Voir target.cpp pour les explications
		{
			glm::mat4 model = glm::translate(glm::rotate(glm::mat4(1.0f), glm::radians(-cubeRotation[j]), glm::vec3(0.0f, 1.0f, 0.0f)), -cubePositions[j]);
			glm::vec3 pPrim = model * glm::vec4(s.pos, 1.0f);
			glm::vec3 pProj = glm::vec3(1.0f);


			for (size_t k = 0; k < 3; ++k)
				if (pPrim[k] < -0.5f)
					pProj[k] = -0.5f;
				else if (pPrim[k] > 0.5f)
					pProj[k] = 0.5f;
				else
					pProj[k] = pPrim[k];

			glm::vec3 pGlob = inverse(model) * glm::vec4(pProj, 1.0f);
			float d = glm::distance(s.pos, pGlob);
			if (d < projectileSys->rayon) s.life = 0.0f;
		}

		//Test de collision avec la cible
		

		if ( target->projCollide(s) )//&& d < projectileSys->rayon)  //Rayon de la cible~2.5
		{
			target->addDeform(s);
		}


	}



}

void MyApp::renderScene() {
	// rendering commands here
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// bind textures on corresponding texture units
	textures["textureDiffuse"].bind(GL_TEXTURE0);
	textures["textureSpecular"].bind(GL_TEXTURE1);
	textures["textureMask"].bind(GL_TEXTURE2);
	textures["ground"].bind(GL_TEXTURE3);
	textures["textureParticle"].bind(GL_TEXTURE6);
	textures["textureTarget"].bind(GL_TEXTURE7);
	textures["textureTarget2"].bind(GL_TEXTURE8);
	textures["textureNormal"].bind(GL_TEXTURE9);

	// be sure to activate shader when setting uniforms/drawing objects
	const Shader& boxShader = shaders["boxShader"];
	boxShader.bind();

	glm::vec3 lightPos(2.0f, 3.0f, 6.0f);	
	//lightPos.x = 2.0f + 6.0f*cos(glfwGetTime()*0.5f);
	//lightPos.z = 3.0f + 6.0f * sin(glfwGetTime()*0.5f);
	//lightPos.y = 3.0f + 0.25*sin(glfwGetTime()*4.0f);
	

	// light properties
	glm::vec3 lightColor(1.0f, 0.1f, 1.0f);  //Fonction de leminscate pour faire varier la couleur de la lumière
	//double t = glfwGetTime()/10;
	//lightColor = glm::vec3( abs(((1 + 0.25 * 1 / t * cos(8 *t)) * cos(0.5 * t)) * sin(0.125*t)) );
	//lightColor.z = abs(sin(glfwGetTime()));

	glm::vec3 diffuseColor = lightColor * glm::vec3(1.0f); // decrease the influence or not...
	glm::vec3 ambientColor = diffuseColor * glm::vec3(0.15f); // low influence

	const Shader& lightProxyShader = shaders["lightProxyShader"];
	lightProxyShader.bind();
	lightProxyShader.setVec3("light.position", lightPos);
	lightProxyShader.setVec3("light.ambient", ambientColor);
	lightProxyShader.setVec3("light.diffuse", diffuseColor);
	lightProxyShader.setVec3("light.specular", lightColor);

	boxShader.bind();
	boxShader.setInt("textureDiffuse", 0);
	boxShader.setInt("textureSpecular", 1);
	boxShader.setInt("textureMask", 2);
	boxShader.setInt("textureNormal", 9);
	boxShader.setVec3("viewPos", camera->Position);
	boxShader.setVec3("eyeDir", camera->Front);
	boxShader.setVec3("light.position", lightPos);
	boxShader.setVec3("light.ambient", ambientColor);
	boxShader.setVec3("light.diffuse", diffuseColor);
	boxShader.setVec3("light.specular", lightColor);

	// material properties
	boxShader.setVec3("material.specular", 1.0f, 1.0f, 1.0f); 
	boxShader.setFloat("material.shininess", 128.0f);


	// projection transformations
	glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)width / (float)height, 0.1f, 100.0f);
	boxShader.setMat4("projection", projection);
	

	// camera/view transformation
	glm::mat4 view = camera->GetViewMatrix();
	boxShader.setMat4("view", view);


	unsigned int nbCubes = 10;
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.5f,  0.0f),
		glm::vec3(1.1f,  0.5f, 0.0f),
		glm::vec3(0.5f, 0.5f, 1.17f),
		glm::vec3(0.5f, 1.5f, 0.5f),
		glm::vec3(2.4f, 0.5f, -3.5f),
		glm::vec3(-1.7f,  0.5f, 3.5f),
		glm::vec3(-1.3f, 0.5f, 2.3f),
		glm::vec3(-2.7f,  0.5f, 2.5f),
		glm::vec3(-2.0f,  1.5f, 2.8f),
		glm::vec3(1.5f,  0.5f, 3.5f)
	};

	float cubeRotation[] = {
		0.0f,
		-4.0f,
		10.0f,
		-25.0f,
		-70.0f,
		-10.0f,
		15.0f,
		-30.0f,
		-35.0f,
		45.0f
	};

	
	// cubes colors
	glm::vec3 cubeColors[] = {
		glm::vec3(1.0f, 0.5f, 0.31f),
		glm::vec3(0.18f, 0.73f, 0.69f),
		glm::vec3(0.97f, 0.79f, 0.31f),
		glm::vec3(0.45f, 0.6f, 0.51f),
		glm::vec3(0.72f, 0.22f, 0.22f),
		glm::vec3(0.36f, 0.83f, 0.36f),
		glm::vec3(0.0f, 0.51f, 0.76f),
		glm::vec3(0.87f, 0.56f, 0.64f),
		glm::vec3(0.99f, 0.65f, 0.02f),
		glm::vec3(0.95f, 1.0f, 0.9f)
	};

	

	testCollision(cubePositions, cubeRotation, nbCubes);

	

	glm::mat4  model = glm::mat4(1.0f);

	// glBindVertexArray(VAO);
	meshes["box"].bind();

	for (unsigned int i = 0; i < nbCubes; i++) {
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		model = glm::translate(model, cubePositions[i]);
		model = glm::rotate(model, glm::radians(cubeRotation[i]), glm::vec3(0.0f, 1.0f, 0.0f));
		boxShader.setMat4("model", model);

		boxShader.setVec3("material.ambient", glm::vec3(1.0));
		boxShader.setVec3("material.diffuse", cubeColors[i]);

		meshes["box"].render();
	}

	// also draw the lamp object
	lightProxyShader.bind();
	lightProxyShader.setMat4("projection", projection);
	lightProxyShader.setMat4("view", view);
	model = glm::mat4(1.0f);
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.1f)); // a smaller cube
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 1.0f)); // a smaller cube	
	lightProxyShader.setMat4("model", model);

	const glMesh& lightProxy = meshes["lightProxy"];
	lightProxy.bind();
	lightProxy.render();
	
	
	const Shader &groundShader = shaders["groundShader"];
	groundShader.bind();
	groundShader.setInt("ground", 3);
	groundShader.setMat4("projection", projection);
	groundShader.setMat4("view", view);
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(100.0f)); 
	groundShader.setMat4("model", model);

	groundShader.setVec3("viewPos", camera->Position);
	groundShader.setVec3("light.position", lightPos);
	groundShader.setVec3("light.ambient", ambientColor);
	groundShader.setVec3("light.diffuse", diffuseColor);
	groundShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	groundShader.setVec3("material.diffuse", 0.3f, 0.7f, 0.8f);
	groundShader.setVec3("material.specular", 1.0f, 0.0f, 1.0f);
	groundShader.setFloat("material.shininess", 128.0f);	
	
	const glMesh& ground = meshes["ground"];
	ground.bind();
	ground.render();

	//setup du shader pour la cible

	const Shader& targetShader = shaders["targetShader"];

	target->bindShaderTarget(targetShader, view, projection);
	const glMesh& targetM = target->modele3D;
	//targetM.bind();
	//targetM.render();
	target->render();
	target->simulate(glfwGetTime());
	
	//Setup des shaders pour les particules
	glEnable(GL_BLEND);  //Activation de l'alpha channel pour activer la transparence
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	const Shader& particleShader = shaders["particleShader"];
	particleShader.bind();
	particleShader.setInt("textureParticle", 6);
	particleShader.setVec3("CameraRight_worldspace", camera->GetViewMatrix()[0][0], camera->GetViewMatrix()[1][0], camera->GetViewMatrix()[2][0]);
	particleShader.setVec3("CameraUp_worldspace", camera->GetViewMatrix()[0][1], camera->GetViewMatrix()[1][1], camera->GetViewMatrix()[2][1]);

	particleShader.setMat4("VP", projection*camera->GetViewMatrix());

	particuleSys->render();
	
	const Shader& projectileShader = shaders["projectileShader"];
	projectileShader.bind();
	model = glm::mat4(1.0f);
	projectileShader.setMat4("model", model);
	projectileShader.setMat4("view", view);
	projectileShader.setMat4("projection", projection);
	projectileShader.bind();
	projectileSys->render();
}

MyApp::~MyApp() {
	if (camera)
		delete camera;
	delete projectileSys, target, particuleSys;
}

void MyApp::onResize(int _width, int _height) {
	width = _width;
	height = _height;

	glViewport(0, 0, width, height);
	frameBufferConfiguration();
}

