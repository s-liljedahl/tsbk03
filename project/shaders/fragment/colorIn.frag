#version 150

out vec4 outColor;
in vec3 exNormalG; // Phong
in vec4 color;
uniform float index;

void main(void)
{
	outColor = color;
}