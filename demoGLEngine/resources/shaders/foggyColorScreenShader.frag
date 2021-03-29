#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D screenDepth;


struct ViewPort {
	float near;
	float far;
};

struct Fog {
	vec3 color;
	float density;
};


uniform ViewPort viewPort;
uniform Fog fog;

float LinearizeDepth(float depth, float near, float far) {
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));
}
		

void main()
{
	float z = LinearizeDepth(texture(screenDepth, TexCoords).r, viewPort.near, viewPort.far);

	const float LOG2 = 1.442695;
	float fogFactor = clamp( exp2( -fog.density * fog.density * z * z * LOG2 ), 0.0, 1.0) ;

    vec3 scenceColor = texture(screenTexture, TexCoords).rgb;
	FragColor = mix( vec4(fog.color, 1.0), vec4(scenceColor, 1.0), fogFactor) ;

} 