#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 eyeDir;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

uniform sampler2D textureDiffuse;
uniform sampler2D textureSpecular;
uniform sampler2D textureMask;
uniform sampler2D textureNormal;

in vec3 FragPos;
in vec3 Normal; 
in vec2 TexCoord;
in mat3 TBN;

void main() {  

    vec3 mixedDiff = vec3( mix( texture(textureDiffuse,TexCoord).xyz * material.diffuse, texture(textureSpecular,TexCoord).xyz , texture(textureMask,TexCoord).r ) );

    //Ambient color
    vec3 ambient = light.ambient * material.diffuse * mixedDiff;



    //Diffuse color
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    //Calculs relatifs à la normale
    //texture relative au calcul de la tangente
    vec3 textureNormal_TS = normalize(texture( textureNormal, vec2(TexCoord.x,TexCoord.y) ).rgb*2.0 - 1.0);
    vec3 lightDirTangent = TBN * lightDir;

    vec3 viewPosTangent = TBN * viewPos;
    vec3 l = normalize(lightDirTangent);
    float cosTheta = clamp( dot(textureNormal_TS,l), 0, 1);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = mixedDiff * cosTheta;

    
    float d = length(light.position - viewPos);
    //Specular
    float specularStrength = 1.0f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDirTangent, norm);  

    float cosAlpha = clamp( dot(viewDir,reflectDir), 0, 1);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(textureSpecular,TexCoord).xyz / d*d ;  // division par pow(distance,2)
    
    

    vec3 result = ( diffuse+ambient + specular);
    FragColor = vec4(result, 1.0);

    
}