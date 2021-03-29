#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
//layout (location = 3) in vec3 vecDeform;
//layout (location = 4) in float rayonDeform;
//layout (location = 5) in vec3 centreDeform;

//layout (location = 3) in vec4 centreDeform;  //Coordonnées x,y,z + rayon en flottant
//layout (location = 4) in vec3 vecteurDeform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float rayonDeform;
uniform int nbDeform;
uniform vec3 vecDeform [10];  //Nombre de déformation maximale en simultanée sur la cible
uniform vec3 centreDeform [10];


out float change;  //Utiliser pour dire au fragment de mettre la texture d'impact
out vec2 TexCoord;


void main(){
        gl_Position = projection * view * model * vec4( aPos , 1.0); 
  
        TexCoord = aTexCoord;

        change = 0.0f;
        for( int i=0; i<nbDeform ; ++i)
        {
            if( pow(aTexCoord.s-centreDeform[i].x , 2) + pow(aTexCoord.t-centreDeform[i].y , 2) < rayonDeform/40.0f)
            {

                float d = distance(aTexCoord,centreDeform[i].xy);  //Déformation linéaire en fonction de la distance des vertexs et du centre de la déformation
                float rD = (rayonDeform*10.0f) - d;
                float def = pow( rD,2);
                gl_Position = projection * view * model * vec4( aPos + vecDeform[i] * def , 1.0);
                change = 1.0f;
            }

        }

    
}