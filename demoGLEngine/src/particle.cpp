
#include "demoGLEngine/particle.hpp"


ParticleSystem::ParticleSystem(glm::vec3 _pos)
	: particlesContainer(std::make_unique<Particle[]>(maxParticles)), g_particule_position_size_data(nullptr), g_particule_color_data(nullptr), g_particule_life_data(nullptr) , g_particule_uv_data(nullptr)
{   //Remplissage des buffers pour le shader
    g_particule_position_size_data = new GLfloat[maxParticles * 4];
    g_particule_color_data = new GLubyte[maxParticles * 4];
    g_particule_life_data = new GLfloat[maxParticles];
    g_particule_uv_data = new GLfloat[maxParticles * 12];
    pos = _pos;
}

ParticleSystem::~ParticleSystem()
{
    delete[] g_particule_position_size_data, g_particule_color_data, g_particule_life_data, g_particule_uv_data;
    release();
}


int ParticleSystem::FindUnusedParticle()
{  //Trouver les particules inutilisées

    for(int i=lastUsedParticle; i<maxParticles; i++){
        if (particlesContainer[i].currentLife <= 0){
            lastUsedParticle = i;
            return i;
        }
    }

    for(int i=0; i<lastUsedParticle; i++){
        if (particlesContainer[i].currentLife <= 0){
            lastUsedParticle = i;
            return i;
        }
    }

	return 0;
}

void ParticleSystem::sortParticles()
{
    std::sort(&particlesContainer[0], &particlesContainer[maxParticles]);
}

void ParticleSystem::init()
{
    //Fonction d'initialisation des buffers
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    //Position des vertices dans le buffer
    static const GLfloat g_vertex_buffer_data[] = 
    { 
		 -0.5f, -0.5f, 0.0f,
		  0.5f, -0.5f, 0.0f,
		 -0.5f,  0.5f, 0.0f,
		  0.5f,  0.5f, 0.0f
	};

    
    glGenBuffers(1, &billboard_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);  
    glEnableVertexAttribArray(0);

    // The VBO containing the positions and sizes of the particles
    glGenBuffers(1, &particles_position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
    glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLfloat), g_particule_position_size_data, GL_STREAM_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &particles_color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
    glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLubyte), g_particule_color_data, GL_STREAM_DRAW);
    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0);  //Transformation des unsigned en vec4 dans le shader
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &particles_life_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_life_buffer);
    glBufferData(GL_ARRAY_BUFFER, maxParticles * sizeof(GLfloat), g_particule_life_data, GL_STREAM_DRAW); 
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, (void*)0 );  
    glEnableVertexAttribArray(3);

    glGenBuffers(1, &particles_uv_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_uv_buffer);
    glBufferData(GL_ARRAY_BUFFER, maxParticles * 12 * sizeof(GLfloat), g_particule_uv_data, GL_STREAM_DRAW);  //12 flottants par UV, 2 coords * 3 points * 2 triangles
    glVertexAttribPointer(4,2, GL_FLOAT, GL_FALSE, 0, (void*)0);  //Seul 2 coordonnées UV sont envoyées au shader
    glEnableVertexAttribArray(4);

    glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
    glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
    glVertexAttribDivisor(2, 1); // color : one per quad                                  -> 1
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 0);
}

//Fonction pour simuler le comportement des particules
void ParticleSystem::simulate(const double & delta, glm::vec3 cameraPosition)
{

    // Generate 10 new particule each millisecond,
   // but limit this to 16 ms (60 fps), or if you have 1 long frame (1sec),
   // newparticles will be huge and the next frame even longer.
    int newparticles = 1;//(int)(delta * 10000.0);
    if (newparticles > (int)(0.016f * 10000.0))
        newparticles = (int)(0.016f * 10000.0);

    for (int i = 0; i < newparticles; i++) {
        int particleIndex = FindUnusedParticle();
        particlesContainer[particleIndex].currentLife = 1.0f; // This particle will live 1 seconds.
        particlesContainer[particleIndex].pos = pos;

        float spread = 1.7f;
        glm::vec3 maindir = glm::vec3(0.0f, 7.0f, 0.0f);

        glm::vec3 randomdir = glm::vec3(
            (rand() % 2000 - 1000.0f) / 1000.0f,
            (rand() % 2000 - 1000.0f) / 1000.0f,
            (rand() % 2000 - 1000.0f) / 1000.0f
        );

        particlesContainer[particleIndex].speed = maindir + randomdir * spread;


        // Very bad way to generate a random color
        particlesContainer[particleIndex].color.r = rand() % 256;
        particlesContainer[particleIndex].color.g = rand() % 256;
        particlesContainer[particleIndex].color.b = rand() % 256;
        particlesContainer[particleIndex].color.a = (rand() % 256) / 3;

        particlesContainer[particleIndex].size = (rand() % 1000) / 2000.0f + 0.1f;

    }


    // Simulate all particles
    particlesCount = 0;
    for (int i = 0; i < maxParticles; i++) {

        Particle& p = particlesContainer[i]; // shortcut

        if (p.currentLife > 0.0f) {

            // Decrease life
            p.currentLife -= delta;
            if (p.currentLife > 0.0f) {

                // Simulate simple physics : gravity only, no collisions
                p.speed += glm::vec3(0.0f, -9.81f, 0.0f) * (float)delta * 0.5f;
                p.pos += p.speed * (float)delta;
                p.cameraDistance = glm::length2(p.pos - cameraPosition);
                //ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;

                // Fill the GPU buffer
                g_particule_position_size_data[4 * particlesCount + 0] = p.pos.x;
                g_particule_position_size_data[4 * particlesCount + 1] = p.pos.y;
                g_particule_position_size_data[4 * particlesCount + 2] = p.pos.z;

                g_particule_position_size_data[4 * particlesCount + 3] = p.size;

                g_particule_color_data[4 * particlesCount + 0] = p.color.r;
                g_particule_color_data[4 * particlesCount + 1] = p.color.g;
                g_particule_color_data[4 * particlesCount + 2] = p.color.b;
                g_particule_color_data[4 * particlesCount + 3] = p.color.a;

                g_particule_life_data[particlesCount] = p.currentLife;

                //UV
                int nbTex = cols * lines;  //40 avec 8 et 5
                int dimY = 0;
                //int dimX = 1;
                //if (dimX == 0) ++dimY;
                int dimX = (int( (1.0f - p.currentLife)  * (float)(nbTex) )  );  //Compris entre 0 et 40

                if (int(particlesContainer[0].currentLife * 100.0) % 2 == 0) {
                    ++cptX;  //Utilisation de variables globales => palier bug, particules toutes la meme textures
                    if (cptX % cols == 0) ++cptY;
                }
                float uv_x = 1.0f / cols * (float)(cptX%cols);  //incrémentation des compteurs

                float uv_y = 1.0f / lines * (float)(cptY%lines);

                //Insertion des vec2 dans le tableau d'UV
                glm::vec2 uv_up_left = glm::vec2(uv_x, uv_y);
                glm::vec2 uv_up_right = glm::vec2(uv_x + 1.0f / cols, uv_y);
                glm::vec2 uv_down_right = glm::vec2(uv_x + 1.0f / cols, (uv_y + 1.0f / lines) ) ;
                glm::vec2 uv_down_left = glm::vec2(uv_x,(uv_y + 1.0f / lines));

                g_particule_uv_data[12 * particlesCount + 0] = uv_up_left.x;
                g_particule_uv_data[12 * particlesCount + 1] = uv_up_left.y;
                g_particule_uv_data[12 * particlesCount + 2] = uv_down_left.x;
                g_particule_uv_data[12 * particlesCount + 3] = uv_down_left.y;
                g_particule_uv_data[12 * particlesCount + 4] = uv_up_right.x;
                g_particule_uv_data[12 * particlesCount + 5] = uv_up_right.y;

                g_particule_uv_data[12 * particlesCount + 6] = uv_down_right.x;
                g_particule_uv_data[12 * particlesCount + 7] = uv_down_right.y;
                g_particule_uv_data[12 * particlesCount + 8] = uv_up_right.x;
                g_particule_uv_data[12 * particlesCount + 9] = uv_up_right.y;
                g_particule_uv_data[12 * particlesCount + 10] = uv_down_left.x;
                g_particule_uv_data[12 * particlesCount + 11] = uv_down_left.y; 

            }
            else {
                // Particles that just died will be put at the end of the buffer in SortParticles();
                p.cameraDistance = -1.0f;
            }
            particlesCount++;
        }
        sortParticles();
    }  

} 

void ParticleSystem::render()
{  //bind du VBO et des buffers
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
    glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLfloat), g_particule_position_size_data, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.

    glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
    glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLubyte), g_particule_color_data, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.

    glBindBuffer(GL_ARRAY_BUFFER, particles_life_buffer);
    glBufferData(GL_ARRAY_BUFFER, maxParticles * sizeof(GLfloat), g_particule_life_data, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
    
    glBindBuffer(GL_ARRAY_BUFFER, particles_uv_buffer);
    glBufferData(GL_ARRAY_BUFFER, maxParticles * 12 * sizeof(GLfloat), g_particule_uv_data, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
    
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particlesCount);
}

void ParticleSystem::release()  //Fonction pour detruire les buffers liés aux projectiles
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &particles_position_buffer);
    glDeleteBuffers(1, &particles_color_buffer);
    glDeleteBuffers(1, &particles_life_buffer);
    glDeleteBuffers(1, &particles_uv_buffer);

}
