#include "glengine/glmesh.hpp"
#include <iostream>

glMesh::glMesh()
	:meshVAO(0), meshVBO(0), positionVBO(0), meshEBO(0), texCoordVBO(0), normalVBO(0), indicesCount(0), verticesCount(0), usage(GL_STATIC_DRAW) {
}

glMesh::glMesh(unsigned int _verticesCount, unsigned int _indicesCount, GLenum _usage)
	:meshVAO(0), meshVBO(0), positionVBO(0), meshEBO(0), texCoordVBO(0), normalVBO(0), verticesCount(_verticesCount), indicesCount(_indicesCount), usage(_usage) {
}

glMesh::glMesh(const std::vector<float>& vertices, const std::vector<float>& normals, const std::vector<float>& texCoords, const std::vector<unsigned int>& indices, GLenum _usage)
	:glMesh(vertices.size()/3, indices.size(), _usage) {
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glGenVertexArrays(1, &meshVAO);
	glBindVertexArray(meshVAO);

	GLuint verticesBufferSize = verticesCount * 3.0 * sizeof(float);

	// Faces
	glGenBuffers(1, &meshEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), usage);

	// position attribute
	glGenBuffers(1, &positionVBO);
	glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), usage);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// normal attribute	
	if (normals.size()) {
		glGenBuffers(1, &normalVBO);
		glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), usage);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
	}	

	// u,v attribute
	if (texCoords.size()) {
		glGenBuffers(1, &texCoordVBO);
		glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
		glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float), texCoords.data(), usage);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(2);
	}	
}


glMesh::glMesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, GLenum _usage)
	:glMesh(vertices.size()/3, indices.size(), _usage) {
	glGenVertexArrays(1, &meshVAO);

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(meshVAO);

	glGenBuffers(1, &meshVBO);
	glBindBuffer(GL_ARRAY_BUFFER, meshVBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), usage);

	glGenBuffers(1, &meshEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), usage);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// u,v attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

glMesh::~glMesh() {
	release();
}


glMesh::glMesh(glMesh&& other) noexcept
	:meshVAO(other.meshVAO), meshVBO(other.meshVBO), positionVBO(other.positionVBO), meshEBO(other.meshEBO), texCoordVBO(other.texCoordVBO), normalVBO(other.normalVBO), indicesCount(other.indicesCount), verticesCount(other.verticesCount), usage(other.usage) {
	other.meshVAO = 0;
	other.meshVBO = 0;
	other.positionVBO = 0;
	other.meshEBO = 0;
	other.texCoordVBO = 0;
	other.normalVBO = 0;
}

glMesh& glMesh::operator=(glMesh&& other) noexcept {
	//ALWAYS check for self-assignment.
	if (this != &other) {
		release();

		//obj_ is now 0.
		std::swap(meshVAO, other.meshVAO);
		std::swap(meshVBO, other.meshVBO);
		std::swap(meshEBO, other.meshEBO);
		std::swap(positionVBO, other.positionVBO);
		std::swap(texCoordVBO, other.texCoordVBO);
		std::swap(normalVBO, other.normalVBO);
		std::swap(indicesCount, other.indicesCount);
		std::swap(verticesCount, other.verticesCount);
		std::swap(usage, other.usage);
	}
	return *this;
}

void glMesh::bind() const {
	glBindVertexArray(meshVAO);
}


void glMesh::release() {
	glDeleteVertexArrays(1, &meshVAO);
	glDeleteBuffers(1, &meshVBO);
	glDeleteBuffers(1, &meshEBO);
	glDeleteBuffers(1, &positionVBO);
	glDeleteBuffers(1, &texCoordVBO);
	glDeleteBuffers(1, &normalVBO);

	meshVAO = 0;
	meshVBO = 0;
	positionVBO = 0;
	meshEBO = 0;
	texCoordVBO = 0;
	normalVBO = 0;
}