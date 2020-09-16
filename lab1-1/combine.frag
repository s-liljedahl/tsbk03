#version 150

uniform sampler2D texUnit;
uniform sampler2D texUnit2;
in vec2 texCoord;
out vec4 fragColor;

void main(void)
{
	vec4 a = texture(texUnit, texCoord);
	vec4 b = texture(texUnit2, texCoord);
	fragColor = (a*0.3 + b*1.0);
}