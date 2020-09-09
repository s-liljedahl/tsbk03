#version 150

in  vec3 inPosition;
in vec2 inTexCoord;
out vec2 texCoord;
out vec2 screenCoord;

uniform mat4 mdlMatrix;
uniform mat4 camMatrix;
uniform mat4 projMatrix;

void main(void)
{
	mat3 normalMatrix1 = mat3(camMatrix * mdlMatrix);

	gl_Position = projMatrix * camMatrix * mdlMatrix * vec4(inPosition, 1.0);
	
	texCoord = inTexCoord;
	screenCoord = vec2(gl_Position) / gl_Position.w / 2.0 + vec2(0.5);
}
