#version 150

out vec4 outColor;
in vec3 exNormalG; // Phong
in float shadow;
uniform float index;

void main(void)
{
	if (shadow == 1.0) {
		outColor = vec4(0.0, 0.0, 0.0, 1.0);
	} else {
		const vec3 light = vec3(0.5, 0.58, 0.2);
		float shade = dot(normalize(exNormalG), light);
		outColor = vec4(shade, shade, shade, 1.0);
	}
}

