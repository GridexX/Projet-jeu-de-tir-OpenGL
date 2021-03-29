
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;


float kernel[9];


const float step_w = 1.0 / 800.0;	//taille de l'image=512, attention en dur !
const float step_h = 1.0 / 600.0;

vec2 offset[9];


void main()  //Shader pour faire du flou gaussien 
{
	kernel[0] = 2.0;
	kernel[1] = 0.0;
	kernel[2] = 0.0;
	kernel[3] = 0.0;
	kernel[4] = -1.0;
	kernel[5] = 0.0;
	kernel[6] = 0.0;
	kernel[7] = 0.0;
	kernel[8] = -1.0;

	offset[0] = vec2(-step_w, -step_h);
	offset[1] = vec2(0.0, -step_h);
	offset[2] = vec2(step_w, -step_h);
	offset[3] = vec2(-step_w, 0.0);
	offset[4] = vec2(0.0, 0.0);
	offset[5] = vec2(step_w, 0.0);
	offset[6] = vec2(-step_w, step_h);
	offset[7] = vec2(0.0, step_h);
	offset[8] = vec2(step_w, step_h);

	vec4 sum = vec4(0.0);

	for( int i = 0; i < 9; i++ )
		{
			vec4 tmp = texture2D(screenTexture, TexCoords.st + offset[i]);
			float luminosite = 0.299 * tmp.r + 0.587 * tmp.g + 0.114 * tmp.b;

			//tmp.rgb = vec3(luminosite);

			sum += tmp * 1/64 * kernel[i];
		}

	FragColor = sum;
}