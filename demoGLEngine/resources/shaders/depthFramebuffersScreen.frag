#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D screenDepth;

struct ViewPort {
	float near;
	float far;
};

uniform ViewPort viewPort;
  
float LinearizeDepth(float depth, float near, float far) {
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    float depth = LinearizeDepth(texture(screenDepth, TexCoords).r, viewPort.near, viewPort.far) / viewPort.far;
    FragColor = vec4(vec3(depth), 1.0);
} 