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

#include "objetsOFF.h"
#include "projectileSystem.hpp"
#include "glengine/glmesh.hpp"

struct Deform  //Structure contenant la déformation
{
	glm::vec3 center;
	glm::vec3 vector;
	glm::vec3 color;
	float radius;
};

class Target {
	
	public:
		//données de la cible dans l'espace
		glm::vec3 pos;
		glm::vec3 scale; 
		glm::vec3 rot, rotAxes;
		glm::mat4 model;

		//Données liées aux déformations
		std::vector<Deform> deforms;
		float* projectile_vecDeform_data;
		float* projectile_centreDeform_data;

		Objet *cible;

		glMesh modele3D;

		//Nombre de deform simultanées sur la cible
		const unsigned int NB_DEFORM_MAX = 10;
		unsigned short indDeform = 0;

		Target(const char* pathObjOff, glm::vec3 _pos = glm::vec3(0.0f,3.0f,0.0f), glm::vec3 _scale = glm::vec3(0.05f), glm::vec3 _rot = glm::vec3(0.0f, 0.0f, 0.0f));
		~Target();

		void loadBuffer();

		void addDeform(const Sphere& s);
		void render();
		void bindShaderTarget(const Shader&, glm::mat4 view, glm::mat4 projection);
		void simulate(const double &);

		bool projCollide(Sphere& s);
};