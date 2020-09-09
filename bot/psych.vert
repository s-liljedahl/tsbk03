#version 150

in  vec3 inPosition;
in vec2 inTexCoord;
out vec2 texCoord;

uniform mat4 modelviewMatrix;
uniform mat4 projMatrix;

void main(void)
{
	texCoord = inTexCoord;

	gl_Position = projMatrix * modelviewMatrix * vec4(inPosition, 1.0);
}
