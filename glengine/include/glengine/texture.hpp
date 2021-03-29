#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad/glad.h>
#include <gl/GL.h>


class Texture {
public:
	Texture(GLenum _wrapS = GL_REPEAT, GLenum _wrapT = GL_REPEAT, GLenum _minFilter = GL_LINEAR_MIPMAP_LINEAR, GLenum _magFilter = GL_LINEAR);
	Texture(const char* texturePath, GLenum _wrapS = GL_REPEAT, GLenum _wrapT = GL_REPEAT, GLenum _minFilter = GL_LINEAR_MIPMAP_LINEAR, GLenum _magFilter = GL_LINEAR);
	~Texture();

	//Delete the copy constructor/assignment.
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	Texture(Texture&& other) noexcept;
	Texture& operator=(Texture&& other) noexcept;

	void bind(GLenum unit) const;
	GLuint getID() const;	


private:
	void release();
	GLuint ID;
	GLenum minFilter;
	GLenum magFilter;
	GLenum wrapS;
	GLenum wrapT;
};

#endif
