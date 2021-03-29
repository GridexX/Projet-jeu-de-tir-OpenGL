#ifndef GLMESH_HPP
#define GLMESH_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <gl/GL.h>
#include <string>
#include <vector>


class glMesh {
public:
    glMesh();    
    glMesh(const std::vector<float>& vertices, const std::vector<float>& normals, const std::vector<float>& texCoords, const std::vector<unsigned int>& indices, GLenum _usage = GL_STATIC_DRAW);
    glMesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, GLenum _usage = GL_STATIC_DRAW);
    ~glMesh();

    //Delete the copy constructor/assignment.
    glMesh(const glMesh&) = delete;
    glMesh& operator=(const glMesh&) = delete;

    glMesh(glMesh&& other) noexcept;
    glMesh& operator=(glMesh&& other) noexcept;

    void bind() const;
    inline GLuint getIndicesCount() const { return indicesCount; }
    inline GLuint getVerticesCount() const { return verticesCount; }
    inline void render() const { glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0); }

private:    
    glMesh(unsigned int _verticesCount, unsigned int _indicesCount, GLenum _usage);
    void release();

    GLuint meshVAO;
    GLuint meshVBO;
    GLuint positionVBO;
    GLuint meshEBO;
    GLuint texCoordVBO;
    GLuint normalVBO;

    GLenum usage;

    GLuint indicesCount;
    GLuint verticesCount;
};

#endif