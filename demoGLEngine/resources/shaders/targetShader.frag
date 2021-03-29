#version 330 core
out vec4 FragColor;

uniform sampler2D textureTarget;
uniform sampler2D textureTarget2;



in vec2 TexCoord;

in float change;


void main() {  
    vec4 Texture= vec4(0.0f);
    
    //Texture de la cible dépassant un certain rayon
    if( pow(TexCoord.t-0.5 , 2) + pow(1-TexCoord.s-0.5 , 2) > 0.1)
        Texture =  texture(textureTarget2,TexCoord);
    else  //sinon image de Mr RAFFIN au centre
        Texture =  texture(textureTarget,TexCoord);
    //Couleur des impacts
    vec4 red = vec4(1.0,0.0,0.0,1.0);
    //Affichage de la texture ou de la couleur de l'impact 
    FragColor = mix ( Texture, red, change);

   
}
