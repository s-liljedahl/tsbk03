#version 150

out vec4 outColor;
in vec2 texCoord;
in vec2 screenCoord;
uniform sampler2D tex, rtftex;
uniform float k;

void main(void)
{
//	outColor = texture(tex, texCoord) * 0.1 + texture(rtftex, screenCoord) * 0.9;
	outColor = texture(tex, texCoord) * (1.0 - k) + texture(rtftex, screenCoord) * k;
}
