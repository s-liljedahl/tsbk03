
#version 150

layout(triangles) in;
layout(triangle_strip, max_vertices = 16) out;

in vec3 exNormal[3];
in vec3 exSurface[3];

uniform mat4 projectionMatrix;
uniform float side;

out vec3 exNormalG;
out vec4 color;

const float magnitude = 0.05;

void makeSquare(vec4 center, vec4 d_x, vec4 d_y, vec4 sideColor) {

	vec4 z = vec4(0.0, 0.0, 1.0, 0.0);
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
	vec4 pos = gl_in[0].gl_Position;

	vec4 dx = vec4(1.0, 0.0, 0.0, 0.0) * magnitude;
	vec4 dy = vec4(0.0, 1.0, 0.0, 0.0) * magnitude;
	vec4 dz = vec4(0.0, 0.0, 0.1, 0.0) * magnitude;

	sideColor = vec4(0.0, 0.1, 0.1, 0.5);
	makeSquare(pos + dy, dx, dz, sideColor);

	sideColor = vec4(0.1, 1.0, 0.1, 0.5);
	makeSquare(pos - dy, dx, dz, sideColor);

	sideColor = vec4(0.5, 0.1, 0.1, 0.5);
	makeSquare(pos - dz, dx, dy, sideColor);

	sideColor = vec4(0.5, 0.1, 0.1, 0.5);
	makeSquare(pos + dz, dx, dy, sideColor);

	makeSquare(pos + dx, dz, dy, sideColor);
	
	makeSquare(pos - dx, dz, dy, sideColor);

	// color = vec4(0.1, 0.1, 1.0, 0.5);
}
