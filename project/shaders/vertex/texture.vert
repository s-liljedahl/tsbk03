#version 150

in vec3 inPosition;
in vec3 inNormal;
in vec2 in_TexCoord;

out vec2 texCoord;

out vec3 exNormal; // Phong
out vec3 exSurface;

uniform mat4 projectionMatrix;
uniform mat4 modelToWorldToView;


void main(void)
{
	// Rotate normal (simple normal matrix, uniform scaling only)
	exNormal = mat3(modelToWorldToView) * inNormal; 
	exSurface = vec3(1.0);
	texCoord = inPosition.st;
	gl_Position = projectionMatrix * modelToWorldToView * vec4(inPosition, 1.0);
}

