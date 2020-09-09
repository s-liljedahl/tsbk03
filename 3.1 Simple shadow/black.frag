#version 150

out vec4 outColor;
//in vec3 exNormal; // Phong
//in vec3 exSurface; // Phong (specular)

void main(void)
{
	float shade = 0.0;
	outColor = vec4(shade, shade, shade, 1.0);
}
