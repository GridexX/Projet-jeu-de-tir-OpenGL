#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()  //Shader correspondant au filtre de vision de nuit
{
	
	vec3 sceneColor = texture(screenTexture, TexCoords).rgb;
	//coef de luminosité
	vec3 lumiCoef = vec3(0.2126, 0.7152, 0.0722);
	float nvg = dot(lumiCoef, sceneColor);

	//modification du niveau de gris, sur le vert
	vec4 greenFilter = vec4(0.0, nvg, 0.0, 1.0);
    vec4 blackColor = vec4(vec3(0.0),1.0);
	//Vignettage noir sur les bords
	FragColor = mix( greenFilter, blackColor, exp(pow(TexCoords.x-0.5,2) + pow(TexCoords.y-0.5,2))*3-3 );
	blackColor = vec4(vec3(0.4),1.0);
	//croix grise de visée au milieu
	if( TexCoords.x >= 0.499 && TexCoords.x <= 0.501  && TexCoords.y >= 0.47 && TexCoords.y <= 0.53 )
		FragColor = blackColor;
	if( TexCoords.x >= 0.47 && TexCoords.x <= 0.53  && TexCoords.y >= 0.499 && TexCoords.y <= 0.501 )
		FragColor = blackColor;
	

} 