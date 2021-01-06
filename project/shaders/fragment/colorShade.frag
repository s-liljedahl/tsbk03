#version 150

out vec4 outColor;
in vec3 exNormalG; // Phong
uniform float index;
in vec4 color;

void main(void)
{
	const vec3 light = vec3(0.5, 0.58, 0.2);
	float shade = dot(normalize(exNormalG), light);
	outColor = color * vec4(shade, shade, shade, 1.0);
}

