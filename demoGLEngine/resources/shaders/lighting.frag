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

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

uniform sampler2D textureDiffuse;
uniform sampler2D textureSpecular;
uniform sampler2D textureMask;

in vec3 FragPos;
in vec3 Normal; 
in vec2 TexCoord;

//Shader des cubes
void main() {  
    
    //Mixage du speculaire et de la diffuse en fonction du masque
    vec3 mixedDiff = vec3( mix( texture(textureDiffuse,TexCoord).xyz * material.diffuse, texture(textureSpecular,TexCoord).xyz , texture(textureMask,TexCoord).r ) );

    //Ambient color
    vec3 ambient = light.ambient * material.diffuse * mixedDiff;

    //Diffuse color
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * mixedDiff;

    //Specular
    float specularStrength = 1.0f;
    //Direction de vue de la cam�ra
    vec3 viewDir = normalize(viewPos - FragPos);
    //Direction de la r�fl�ctance en fonction de la lumi�re et de la normale
    vec3 reflectDir = reflect(-lightDir, norm); 
    //speculaire en fonction du reflet et de la vue, g�r� par la brillance du mat�riau
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(textureSpecular,TexCoord).xyz ;  

    //couleur finale combinant les 3 diff�rents �clairage => mod�le de Phong
    vec3 result = (diffuse + ambient + specular);
    FragColor = vec4(result, 1.0);

    
}
