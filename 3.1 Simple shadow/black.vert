#version 150

in  vec3 inPosition;
in  vec3 inNormal;

uniform mat4 modelviewMatrix;
uniform mat4 projectionMatrix;

void main(void)
{
	gl_Position = projectionMatrix * modelviewMatrix * vec4(inPosition, 1.0); // This should include projection
}
