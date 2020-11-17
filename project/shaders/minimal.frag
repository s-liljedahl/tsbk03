#version 150

out vec4 outColor;
in vec3 exNormalG; // Phong
uniform float index;

void main(void)
{
	const vec3 light = vec3(0.58, 0.58, 0.1);
	float shade;
	shade = dot(normalize(exNormalG), light);
	outColor = vec4(shade, shade, shade, 1.0);
}

