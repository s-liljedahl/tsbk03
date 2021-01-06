
#version 150

layout(triangles) in;
layout(triangle_strip, max_vertices = 24) out;

in vec3 exNormal[3];
in vec3 exSurface[3];

uniform mat4 projectionMatrix;
uniform mat4 modelToWorldToView;
uniform mat4 viewMatrix;
uniform float side;

out vec3 exNormalG;
out vec4 color;

const float magnitude = 0.05;

void makeSquare(vec4 center, vec4 d_x, vec4 d_y, vec4 sideColor) {

	exNormalG = exNormal[0];
	color = sideColor;

	gl_Position = (center + d_x + d_y);
	EmitVertex();   
	gl_Position = (center - d_x + d_y);
	EmitVertex();
	gl_Position = (center + d_x - d_y);
	EmitVertex();   
	gl_Position = (center - d_x - d_y);
	EmitVertex();

	EndPrimitive();
}


void main()
{
	vec4 sideColor;
	vec4 pos = viewMatrix * gl_in[0].gl_Position;

	vec4 dx = magnitude * projectionMatrix * viewMatrix * modelToWorldToView * vec4(1.0, 0.0, 0.0, 0.0);
	vec4 dy = magnitude * projectionMatrix * viewMatrix * modelToWorldToView * vec4(0.0, 1.0, 0.0, 0.0);
	vec4 dz = magnitude * projectionMatrix * viewMatrix * modelToWorldToView * vec4(0.0, 0.0, 1.0, 0.0);

	sideColor = vec4(1.0, 0.1, 0.1, 1.0);
	makeSquare(pos + dy, dx, dz, sideColor);

	sideColor = vec4(0.1, 1.0, 0.1, 1.0);
	makeSquare(pos - dy, dx, dz, sideColor);
	
	sideColor = vec4(0.6, 0.1, 0.1, 1.0);
	makeSquare(pos - dz, dx, dy, sideColor);
	
	sideColor = vec4(0.1, 0.1, 0.6, 1.0);
	makeSquare(pos + dz, dx, dy, sideColor);
	
	sideColor = vec4(0.1, 0.1, 1.0, 1.0);
	makeSquare(pos + dx, dz, dy, sideColor);
	
	sideColor = vec4(0.1, 1.0, 1.0, 1.0);
	makeSquare(pos - dx, dz, dy, sideColor);

}
