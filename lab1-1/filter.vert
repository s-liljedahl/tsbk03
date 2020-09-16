#version 150

in vec3 in_Position;
out vec3 norm;
out vec2 texCoord;

void main(void)
{
	// Eller motsv texturkoordinater
	texCoord = vec2(in_Position / 2.0 + vec3(0.5));
	gl_Position = vec4(in_Position, 1.0);

}