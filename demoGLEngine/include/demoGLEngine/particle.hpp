#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <algorithm>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include "glengine/texture.hpp"


#include <stdexcept>
#include <vector>
#include <iostream>
// #define STB_IMAGE_IMPLEMENTATION
#include "stbimage/stb_image.h"

#include <glad/glad.h>
#include <algorithm>
#include <gl/GL.h>
#include <GLFW/glfw3.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/ext/vector_float2.hpp>
#include <memory>

struct Particle {
	glm::vec3 pos, speed;
	glm::vec4 color;
	glm::vec2 coordText;

	float size, angle, weight;
	float currentLife; // Remaining life of the particle. if <0 : dead and unused.
	float cameraDistance; // *Squared* distance to the camera. if dead : -1.0f

	inline Particle() : currentLife(0.0) {;} ;
	bool operator<(const Particle& that) const {
		// Sort in reverse order : far particles drawn first.
		return this->cameraDistance > that.cameraDistance;
	}
};

class ParticleSystem
{
public:

	ParticleSystem(glm::vec3 _pos=glm::vec3(0.0));
	~ParticleSystem();

	//void update(const double& dt, const glm::vec3 & posView);
	int FindUnusedParticle();
	void sortParticles();
	void simulate(const double& delta, glm::vec3 cameraPosition = glm::vec3(0.0));
	void init();
	void render();
	void release();

private:
	//static constexpr float maxLife = 20.0f;
	static constexpr unsigned int maxParticlePerSecond = 300;
	static constexpr unsigned int maxParticles = 100;
	unsigned int lastUsedParticle = 0;
	unsigned int particlesCount;
	std::unique_ptr<Particle[]> particlesContainer;
	const unsigned int lines = 5;
	const unsigned int cols = 8;
	int cptX = 0;
	int cptY = -1;
	glm::vec3 pos;

	GLfloat* g_particule_position_size_data;
	GLfloat* g_particule_life_data;
	GLubyte* g_particule_color_data;
	GLfloat* g_particule_uv_data;

	GLuint billboard_vertex_buffer;
	GLuint particles_position_buffer;
	GLuint particles_color_buffer;
	GLuint particles_life_buffer;
	GLuint particles_uv_buffer;

	unsigned int VAO;




};