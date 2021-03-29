#include "demoGLEngine/projectileSystem.hpp"

Sphere::Sphere(glm::vec3 _pos, glm::vec3 _speed)
	: pos(_pos), speed(_speed), life(1.0f)//, VAO(0), vertices_buffer(0)
{}

Sphere::~Sphere()
{}

void Sphere::release()  //Destruction des buffers du projectile
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &vertices_buffer);
}

void Sphere::init(std::vector<float> vertices)
{
    //Initialisation des buffers pour la géométrie
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //Buffer des vertexs
    glGenBuffers(1, &vertices_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertices_buffer);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STREAM_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Sphere::update(std::vector<float> vertices, const double& delta)
{
    //Maj de la position en fonction du temps et du vec directeur
    pos += speed * (float)delta * 15.0f;
    std::vector<float> v;
    for (size_t i = 0; i < vertices.size()/3; i++)  //Ajout des points dans le buffer aux bonnes coordonnées
    {
        v.push_back( vertices.at(3*i) + pos.x);
        v.push_back( vertices.at(3*i + 1) + pos.y );
        v.push_back( vertices.at(3*i + 2) + pos.z );
    }
    glBindBuffer(GL_ARRAY_BUFFER, vertices_buffer);
    glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(GLfloat), &v[0], GL_STREAM_DRAW);
}

void Sphere::render(const size_t nbVertices)
{
    glBindVertexArray(VAO);
    glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, nbVertices, nbVertices);
}

void ProjectileSystem::discretiseSphere()  //Discrétisation dans l'ordre des triangles
{
 
    double PI = 3.14159265358979323846;

    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / rayon;    // vertex normal
    float s, t;                                     // vertex texCoord

    float sectorStep = 2 * PI / sectors;
    float stackStep = PI / stacks;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stacks; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = rayon * cosf(stackAngle);             // r * cos(u)
        z = rayon * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= sectors; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }
}



ProjectileSystem::ProjectileSystem()
{
    //Initialisation des vertices pour tous les Mesh de sphere
    discretiseSphere();

}

ProjectileSystem::~ProjectileSystem()

{}

void ProjectileSystem::addProjectiles(glm::vec3 pos, glm::vec3 speed)
{
    if (projectiles.size() < maxProjectiles)
    {
        Sphere s(pos, speed);
        //Init les buffers des projectiles
        s.init(vertices);
        projectiles.push_back(s);
    }
}

void ProjectileSystem::render()
{
    for (size_t i = 0; i < projectiles.size(); ++i)
        projectiles.at(i).render(vertices.size()/3);
}


void ProjectileSystem::simulate(const double& delta)
{ 
    for (size_t i = 0; i < projectiles.size(); ++i)
    {
        Sphere& projectile = projectiles.at(i);
        if(projectile.life > 0.0f)
        {
            projectile.update(vertices, delta);
            //Pour decrease doucement la vie
            projectile.life -= delta * 0.2f;
        }
        else  //Suppression des projectiles morts
        {
            projectile.release();
            projectiles.erase(projectiles.begin() + i);
        }
    } 

}

