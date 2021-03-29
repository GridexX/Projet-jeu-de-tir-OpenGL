#include "glengine/texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stbimage/stb_image.h"
#include <string>
#include <stdexcept>

Texture::Texture(GLenum _wrapS, GLenum _wrapT, GLenum _minFilter, GLenum _magFilter)
:ID(0), wrapS(_wrapS), wrapT(_wrapT), minFilter(_minFilter), magFilter(_magFilter) {
}

Texture::Texture(const char* texturePath, GLenum _wrapS, GLenum _wrapT, GLenum _minFilter, GLenum _magFilter)
	:Texture(_wrapS,_wrapT,_minFilter,_magFilter) {
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

	// load image, create texture and generate mipmaps	
	int textureWidth, textureHeight, textureNrChannels;	
	unsigned char* data = stbi_load(texturePath, &textureWidth, &textureHeight, &textureNrChannels, 0);

	GLenum format = GL_RGBA;
	if (data) {
		switch (textureNrChannels) {
		case 1: format = GL_RED; break;
		case 3: format = GL_RGB; break;
		case 4: format = GL_RGBA;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, format, textureWidth, textureHeight, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		throw std::runtime_error("Failed to load texture");
	}
	stbi_image_free(data);
}

Texture::~Texture() {
	release();
}

Texture::Texture(Texture&& other) noexcept
	:ID(other.ID), wrapS(other.wrapS), wrapT(other.wrapT), minFilter(other.minFilter), magFilter(other.magFilter) {
	other.ID = 0; //Use the "null" texture for the old object.
}

Texture& Texture::operator=(Texture&& other) noexcept {
	//ALWAYS check for self-assignment.
	if (this != &other) {
		release();
		
		//obj_ is now 0.
		std::swap(ID, other.ID);
		std::swap(minFilter, other.minFilter);
		std::swap(magFilter, other.magFilter);
		std::swap(wrapS, other.wrapS);
		std::swap(wrapT, other.wrapT);
	}
	return *this;
}

void Texture::release() {
	glDeleteTextures(1, &ID);
	ID = 0;
}

void Texture::bind(GLenum unit) const {
	glActiveTexture(unit);
	glBindTexture(GL_TEXTURE_2D, ID);
}

GLuint Texture::getID() const {
	return ID;
}


