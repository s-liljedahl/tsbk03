#version 150

in vec3 inPosition;

out vec3 baseColour;

void main(void){

	gl_Position = vec4(inPosition,1.0);
	baseColour = vec3(0.0, 1.0, 1.0);

}