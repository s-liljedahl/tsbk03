#version 150

layout ( triangles ) in;
layout ( triangle_strip, max_vertices = 8 ) out;

in vec3 baseColour[];

out vec3 colour;

uniform mat4 modelToWorldToView;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

const float size = 0.05;

void main(void){

	vec4 offset = vec4(-1.0, 1.0, 0.0, 0.0) * size;
	vec4 vertexPos = offset  + gl_in[0].gl_Position;
	gl_Position = projectionMatrix * viewMatrix * modelToWorldToView * vertexPos;
	colour = baseColour[0] * vec3(1.0, 0.0, 0.0);
	EmitVertex();
	
	offset = vec4(-1.0, -1.0, 0.0, 0.0) * size;
	vertexPos = offset + gl_in[0].gl_Position;
	gl_Position = projectionMatrix * viewMatrix * modelToWorldToView * vertexPos;
	colour = baseColour[0] * vec3(0.0, 1.0, 0.0);
	EmitVertex();
	
	offset = vec4(1.0, 1.0, 0.0, 0.0) * size;
	vertexPos = offset + gl_in[0].gl_Position;
	gl_Position = projectionMatrix * viewMatrix * modelToWorldToView * vertexPos;
	colour = baseColour[0] * vec3(1.0, 0.0, 1.0);
	EmitVertex();
	
	offset = vec4(1.0, -1.0, 0.0, 0.0) * size;
	vertexPos = offset + gl_in[0].gl_Position;
	gl_Position = projectionMatrix * viewMatrix * modelToWorldToView * vertexPos;
	colour = baseColour[0] * vec3(0.0, 0.0, 1.0);
	EmitVertex();
	
	EndPrimitive();
	
	offset = vec4(-1.0, 1.0, 1.0, 0.0) * size;
	vertexPos = offset + gl_in[0].gl_Position;
	gl_Position = projectionMatrix * viewMatrix * modelToWorldToView * vertexPos;
	colour = baseColour[0] * vec3(1.0, 0.0, 0.0);
	EmitVertex();
	
	offset = vec4(-1.0, -1.0, 1.0, 0.0) * size;
	vertexPos = offset + gl_in[0].gl_Position;
	gl_Position = projectionMatrix * viewMatrix * modelToWorldToView * vertexPos;
	colour = baseColour[0] * vec3(0.0, 1.0, 0.0);
	EmitVertex();
	
	offset = vec4(1.0, 1.0, 1.0, 0.0) * size;
	vertexPos = offset + gl_in[0].gl_Position;
	gl_Position = projectionMatrix * viewMatrix * modelToWorldToView * vertexPos;
	colour = baseColour[0] * vec3(1.0, 0.0, 1.0);
	EmitVertex();
	
	offset = vec4(1.0, -1.0, 1.0, 0.0) * size;
	vertexPos = offset + gl_in[0].gl_Position;
	gl_Position = projectionMatrix * viewMatrix * modelToWorldToView * vertexPos;
	colour = baseColour[0] * vec3(0.0, 0.0, 1.0);
	EmitVertex();
	
	EndPrimitive();
}