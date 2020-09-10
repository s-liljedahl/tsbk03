// Revised 2019 with a bit better variable names.

// This worked 2015: 
// Linux: gcc lab0.c ../common/*.c ../common/Linux/MicroGlut.c -lGL -o lab0 -I../common -I../common/Linux
// Mac: gcc lab0.c ../common/*.c ../common/Mac/MicroGlut.m -o lab0 -framework OpenGL -framework Cocoa -I../common/Mac -I../common

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	//uses framework Cocoa
#else
	#include <GL/gl.h>
	#include "MicroGlut.h"
#endif
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

//constants
const int initWidth=512, initHeight=512;

// Model-to-world matrix
// Modify this matrix.
// See below for how it is applied to your model.
mat4 modelToWorld = {{
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0
}};
// World-to-view matrix. Usually set by lookAt() or similar.
mat4 worldToView;
// Projection matrix, set by a call to perspective().
mat4 projectionMatrix;

// Globals
// * Model(s)
Model *bunny;
Model *teddy;
Model *teapot;

// * Reference(s) to shader program(s)
GLuint program;
// * Texture(s)
GLuint texture;


// light sources
vec3 lightSourcesColorsArr[] = {
	{1.0f, 0.0f, 0.0f}, // Red light
	{0.0f, 0.7f, 0.5f}, // Green light
	{0.0f, 0.0f, 1.0f}, // Blue light
	{1.0f, 1.0f, 1.0f} // White light
};
GLint isDirectional[] = {1,1,1,1};

vec3 lightSourcesDirectionsPositions[] = {
	{10.0f, 5.0f, 0.0f}, // Red light, positional
	{0.0f, 5.0f, 10.0f}, // Green light, positional
	{-1.0f, 0.0f, 0.0f}, // Blue light along X
	{0.0f, 0.0f, -1.0f}, // White light along Z
};
// and the specular exponent (per object):
GLfloat specularExponent[] = {200.0, 200.0, 60.0, 50.0, 300.0, 150.0};


void init(void)
{
// three vertex buffer objects, used for uploading the data
	unsigned int normalBufferObjID;

	dumpInfo();

	// GL inits
	glClearColor(0.7,0.6,0.0,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	printError("GL inits"); // This is merely a vague indication of where something might be wrong

	projectionMatrix = perspective(100, 1.0, 0.1, 1000);
	worldToView = lookAt(0,0,1.5, 0,0,0, 0,1,0);

	// Load and compile shader
	program = loadShaders("lab0.vert", "lab0.frag");
	glUseProgram(program);
	printError("init shader");
	
	// Upload geometry to the GPU:
	bunny = LoadModelPlus("objects/stanford-bunny.obj");
	teddy = LoadModelPlus("objects/teddy.obj");
	teapot = LoadModelPlus("objects/teapot.obj");
	printError("load models");

	// Load textures
	LoadTGATextureSimple("textures/maskros512.tga",&texture);
	printError("load textures");

	// Load light sources
	glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
	glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
	glUniform1iv(glGetUniformLocation(program, "isDirectional"), 4, isDirectional);

}

void display(void)
{
	printError("pre display");
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	//activate the program, and set its variables
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);

	GLfloat rs_t = sin(t/1000);
	GLfloat rc_t = cos(t/1000);

	// around x-axis
	mat4 modifiedMTW_x = {{
		1.0, 0.0, 0.0, 0.0,
		0.0, rc_t, -rs_t, 0.0,
		0.0, rs_t, rc_t, 0.0,
		0.0, 0.0, 0.0, 1.0
	}};

	// around y-axis
	mat4 modifiedMTW_y = {{
		rc_t, 0.0, rs_t, 0.0,
		0.0, 1.0, 0.0, 0.0,
		-rs_t, 0.0, rc_t, 0.0,
		0.0, 0.0, 0.0, 1.0
	}};

	mat4 rz = Mult(Rz(t/1000), Ry(t/500));
	mat4 modifiedMTW_z = Mult(rz, modelToWorld); 
	mat4 modelToWorldToView = Mult(worldToView, modifiedMTW_z); // Combine to one matrix

	glUniformMatrix4fv(glGetUniformLocation(program, "modelToWorldToView"), 1, GL_TRUE, modelToWorldToView.m);
	
	glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent[0]);

	//draw the model
	DrawModel(bunny, program, "in_Position", "in_Normal", NULL);
	DrawModel(bunny, program, "in_Position", "in_Normal", NULL);
	
	
	
	printError("display");
	
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("Lab 0 - OpenGL 3.2+ Introduction");
	glutDisplayFunc(display); 
	glutRepeatingTimer(20);
	init ();
	glutMainLoop();
	exit(0);
}

