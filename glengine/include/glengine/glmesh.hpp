#ifndef GLMESH_HPP
#define GLMESH_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <gl/GL.h>
#include <string>
#include <map>
#include <vector>



class glMesh {
public:
    glMesh();    
    glMesh(const std::vector<float>& vertices, const std::vector<float>& normals, const std::vector<float>& texCoords, const std::vector<unsigned int>& indices, GLenum _usage = GL_STATIC_DRAW);
    glMesh(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& texCoords, const std::vector<unsigned int>& indices, const std::vector<glm::vec3>& tangents, const std::vector<glm::vec3>& bitangents,  GLenum _usage = GL_STATIC_DRAW);
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

    void computeTangentBasis(std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals, std::vector<glm::vec3>& tangents, std::vector<glm::vec3>& bitangents);
    bool is_near(float v1, float v2);
    bool getSimilarVertexIndex( glm::vec3& in_vertex, glm::vec2& in_uv, glm::vec3& in_normal, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals, unsigned int& result);
    void indexVBO_TBN(std::vector<glm::vec3>& in_vertices, std::vector<glm::vec2>& in_uvs, std::vector<glm::vec3>& in_normals, std::vector<glm::vec3>& in_tangents, std::vector<glm::vec3>& in_bitangents, std::vector<unsigned int>& out_indices, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals, std::vector<glm::vec3>& out_tangents, std::vector<glm::vec3>& out_bitangents);

private:    
    glMesh(unsigned int _verticesCount, unsigned int _indicesCount, GLenum _usage);
    void release();

    GLuint meshVAO;
    GLuint meshVBO;
    GLuint positionVBO;
    GLuint meshEBO;
    GLuint texCoordVBO;
    GLuint normalVBO;

    GLuint tangentVBO;
    GLuint bitangentVBO;

    GLenum usage;

    GLuint indicesCount;
    GLuint verticesCount;
};

#endif