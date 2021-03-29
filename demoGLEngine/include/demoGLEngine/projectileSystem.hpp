#pragma once
#include <stdio.h>
#include <stdlib.h>

#include <algorithm>

#include "glengine/shader.hpp"
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


class Sphere
{
public:

	glm::vec3 pos, speed; 
	float life;
	GLuint VAO, vertices_buffer;

	Sphere(glm::vec3 _pos = glm::vec3(0.0f), glm::vec3 _speed = glm::vec3(0.0f));
	~Sphere();

	void init(std::vector<float> vertices);
	void update(std::vector<float> vertices, const double& delta);
	void render(const size_t nbVertices);
	void release();
}; 

class ProjectileSystem  //Système gérant les projectiles;
{
public :

	std::vector<Sphere> projectiles;
	std::vector<float> vertices;

	//Données liées à la discrétisation de la sphère
	const float rayon = 0.3f;
	const unsigned int stacks = 10;
	const unsigned int sectors = 10;
	const glm::vec4 color = glm::vec4(1.0, 0.0, 0.0, 0.0);  //projectile de couleur rouge

	const unsigned int maxProjectiles = 30;  //Nombre maximal de projectiles pouvant être tiré en même temps


	ProjectileSystem();
	~ProjectileSystem();

	void discretiseSphere();

	void addProjectiles(glm::vec3 pos, glm::vec3 speed);  //Creer une nouvelles sphères à la bonne position en fonction d'un vecteur directeur
	

	void render();
	void simulate(const double& delta);
};