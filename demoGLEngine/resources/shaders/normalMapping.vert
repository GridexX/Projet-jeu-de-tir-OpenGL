#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 UV;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;
out mat3 TBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	FragPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
	TexCoord = UV;
	Normal = normal;
	mat3 MV3x3 = mat3(view) * mat3(model);
	vec3 vNormalScreen = MV3x3 * normal;
	vec3 vTangentScreen = MV3x3 * tangent;
	vec3 vBitangentScreen = MV3x3 * bitangent;
	TBN = transpose(mat3( vTangentScreen, vBitangentScreen, vNormalScreen ));
}