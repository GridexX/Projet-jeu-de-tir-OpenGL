#version 330 core

// Interpolated values from the vertex shaders

in vec4 pColor;
in float pLife;
in vec2 UV;

out vec4 color;

uniform sampler2D textureParticle;


void main(){
	
    vec4 texColor = vec4(texture(textureParticle,UV));

	color = texColor;

}