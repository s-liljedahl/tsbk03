#version 150

in vec3 exNormalG; // Phong
in vec2 texCoordG;
// in vec3 exNormal; // Phong
// in vec2 texCoord;

uniform sampler2D tex;
uniform float index;

out vec4 outColor;

void main(void)
{
	// float shade = dot(normalize(vec4(exNormalG, 1.0)), texture);
	// outColor = vec4(shade, shade, shade, 1.0);
	outColor = texture(tex, texCoordG);
	// outColor = texture(tex, texCoord);

}

