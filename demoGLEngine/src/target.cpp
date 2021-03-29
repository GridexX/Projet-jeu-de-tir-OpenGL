#include "demoGLEngine/target.hpp"

Target::Target(const char* pathObjOff, glm::vec3 _pos, glm::vec3 _scale, glm::vec3 _rot)
    : pos(_pos), scale(_scale), rot(_rot), projectile_vecDeform_data(nullptr), projectile_centreDeform_data(nullptr), cible(nullptr)
{
    rotAxes = glm::vec3(0.0f, 1.0f, 0.0f);
    model = glm::mat4(1.0f);
    projectile_vecDeform_data = new float[3 * NB_DEFORM_MAX];
    projectile_centreDeform_data = new float[3 * NB_DEFORM_MAX];

    //initialisation des paramètres de la cible
    cible = new Objet();
    cible->charge_OFF(pathObjOff);
}

void Target::loadBuffer()
{
    std::vector<float> targetVertices;
    std::vector<float> targetUV;
    std::vector<unsigned int> targetIndices;

    for (int nbp = 0; nbp < cible->nbsommets; nbp++)
    {
        //Positions des points
        targetVertices.push_back(cible->lpoints[nbp].x); targetVertices.push_back(cible->lpoints[nbp].y); targetVertices.push_back(cible->lpoints[nbp].z);

        //Position des UVs
        targetUV.push_back(cible->lpoints[nbp].v); targetUV.push_back(cible->lpoints[nbp].u);
    }
    for (int nfaces = 0; nfaces < cible->nbfaces; nfaces++)
    {
        //index des faces
        targetIndices.push_back(cible->lfaces[nfaces].S1); targetIndices.push_back(cible->lfaces[nfaces].S2); targetIndices.push_back(cible->lfaces[nfaces].S3);
    }

    //Reverse pour avoir la cible à l'endroit
    std::reverse(targetUV.begin(), targetUV.end());
    
    modele3D = glMesh(targetVertices, {}, targetUV, targetIndices);

}

Target::~Target()
{
    delete cible;
    delete[] projectile_vecDeform_data, projectile_centreDeform_data;
}

void Target::addDeform(const Sphere& s)
{
    Deform d;
    d.center = s.pos;
    d.color = glm::vec4(1.0, 0.0, 0.0, 0.0);
    d.vector = s.speed;
    d.radius = 0.3f;

    //Gère un nombre maximal de déformation
    //Remplace 1ere deformation si liste pleine et suivante après à chaque fois
    if (deforms.size() >= NB_DEFORM_MAX)
    {
        if (indDeform >= NB_DEFORM_MAX) indDeform = 0;
        deforms[indDeform++] = d;
    }
    else {
        deforms.push_back(d);
    }

    //remplissage du buffer pour les données liées à la déformation
    for (size_t i = 0; i < deforms.size() && i < NB_DEFORM_MAX; ++i)
    {
        projectile_vecDeform_data[3 * i + 0] = deforms[i].vector.x;
        projectile_vecDeform_data[3 * i + 1] = deforms[i].vector.y;
        projectile_vecDeform_data[3 * i + 2] = deforms[i].vector.z;

        projectile_centreDeform_data[3 * i + 0] = deforms[i].center.x;
        projectile_centreDeform_data[3 * i + 1] = deforms[i].center.y;
        projectile_centreDeform_data[3 * i + 2] = deforms[i].center.z;
    }
}

void Target::render()
{
    modele3D.bind();
    modele3D.render();
}

//Fonction pour tester la collision avec la cible
bool Target::projCollide(Sphere& s)
{
    //On prend 3 points du plans pour définir le vecteur normal
    glm::vec3 pointMax = model * glm::vec4(cible->max.x, cible->max.y, cible->max.z, 1.0);
    glm::vec3 pointMin = model * glm::vec4(cible->lpoints[700].x, cible->lpoints[700].y, cible->lpoints[700].z, 1.0);
    glm::vec3 vNormal = glm::cross(pos - pointMax, pointMin - pointMax);
    glm::vec3 vecSP = s.pos - pos;

    //Distance orthogonale par rapport au plan 
    float dOrtho = abs(glm::dot(vNormal, vecSP)) / glm::length(vNormal);
    //distance par rapport au centre
    float dSP = glm::distance(s.pos, pos);

    //Projection dans le plan de la cible
    glm::mat4 modelT = glm::translate(glm::rotate(glm::mat4(1.0f), glm::radians(-rot.y), rotAxes), -pos);
    glm::vec3 pPrim = modelT * glm::vec4(s.pos, 1.0f);
    glm::vec3 pProj = glm::vec3(1.0f);
    float rayonCible = 100.0f * scale.x * 0.5f;
    for (size_t k = 0; k < 2; ++k)
        if (pPrim[k] < -rayonCible)
            pProj[k] = -rayonCible;
        else if (pPrim[k] > rayonCible)
            pProj[k] = rayonCible;
        else
            pProj[k] = pPrim[k];

    if (pPrim.z < -0.01f)
        pProj.z = -0.01f;
    else if (pPrim.z > 0.01f)
        pProj.z = 0.01f;
    else
        pProj.z = pPrim.z;

    glm::vec3 pGlob = inverse(modelT) * glm::vec4(pProj, 1.0f);
    float d = glm::distance(s.pos, pGlob);
    
    //on vérifie si la balle est comprise dans le cercle et est suffisamment proche du plan
    if(dOrtho < 0.25 && dSP < rayonCible)
    {
        s.life = 0.0f;
        //maj de la position pour correpondre aux coords UV de la cible [-2.5;2.5] -> [0;1]
        s.pos = 1.0f - (pProj * 0.2f + 0.5f);
    }
    return (dOrtho < 0.25 && dSP < rayonCible);
}

//Bind avec le Shader
void Target::bindShaderTarget(const Shader &shader, glm::mat4 view, glm::mat4 projection)
{
    shader.bind();
    
    glm::mat4 modelT = glm::translate(glm::mat4(1.0f), pos);
    modelT = glm::rotate(modelT, glm::radians(rot.y), rotAxes);
    modelT = glm::scale(modelT, scale);
    model = modelT;
    
    shader.setMat4("model", modelT);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    shader.setInt("textureTarget", 7);
    shader.setInt("textureTarget2", 8);
    shader.setInt("nbDeform", deforms.size());
    shader.setFloat("rayonDeform", 0.3f);
    if (deforms.empty()) return;  //Si il y a des desforms, on bind avec leurs valeurs, sinon stop
    
    shader.setVec3("vecDeform", projectile_vecDeform_data, deforms.size());
    shader.setVec3("centreDeform", projectile_centreDeform_data, deforms.size());
}

//Déplacement suivant "leminscate" -> courbe en 8
void Target::simulate(const double &dt)
{
    float t = (float)dt * 2.0;
    //ajout de random entre chaque valeur de déplacement;
    
    pos.x = (8 + 0.125f * sin(32.0f * t)) * cos(0.5f * t) + 7.0f;
    pos.z = 2.0f * sin(t) - 3.5f;

}