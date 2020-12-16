#version 150

in  vec3 inPosition;
in  vec3 inNormal;

out vec3 exNormal; // Phong
out vec3 exSurface;

uniform mat4 projectionMatrix;
uniform mat4 modelToWorldToView;


void main(void)
{
	// Rotate normal (simple normal matrix, uniform scaling only)
	exNormal = mat3(modelToWorldToView) * inNormal; 
	gl_Position = projectionMatrix * modelToWorldToView * vec4(inPosition, 1.0);
	exSurface = vec3(1.0);
}
