// Simple shadow
// One of the simplest shadows there is is to flatten a shape
// and paint it black on a plane.
// If you want to make this nicer, you should draw the ground
// with some pattern and make the shadow transparent.
// However, the shadow must be completely within the ground plane.
// If it isn't, you need to use the stencil buffer to cut away parts outside.

// By Ingemar 2013

// MS Windows needs GLEW or glee.
#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// linking hint for Lightweight IDE
	//uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include <stdlib.h>

mat4 projectionMatrix;

// Pointer to model data
Model *bunny;

// Reference to shader program
GLuint program, black;

void init(void)
{
	// GL inits
	glClearColor(1,1,1,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	printError("GL inits");

	projectionMatrix = frustum(-0.4, 0.4, -0.4, 0.4, 1.0, 300.0);

	// Load and compile shader
	black = loadShaders("black.vert", "black.frag");
	program = loadShaders("phong.vert", "phong.frag");
	glUseProgram(program);
	printError("init shader");
	
	// Upload geometry to the GPU:
	bunny = LoadModelPlus("stanford-bunny900.obj");
	CenterModel(bunny);
	ScaleModel(bunny, 5, 5, 5);
	ReloadModelData(bunny);

// Send to both shaders!
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUseProgram(black);
	glUniformMatrix4fv(glGetUniformLocation(black, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
	
	printError("init arrays");
}

GLfloat a = 0.0;

void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	Matrix4D worldToView, m1, m2, m, tr, scale;
	worldToView = lookAt(0,0.4,1.8, 0,-0.3,0, 0,1,0);
	a += 0.1; // Uncomment to get a rotating bunny
	
	glUseProgram(program);
	m = Mult(worldToView, Ry(a));
	glUniformMatrix4fv(glGetUniformLocation(program, "modelviewMatrix"), 1, GL_TRUE, m.m);
	DrawModel(bunny, program, "inPosition", "inNormal", NULL);
	
	m = Mult(worldToView, Mult(T(0, -0.8, 0), Mult(S(1, 0, 1), Ry(a))));
	glUseProgram(black);
	glUniformMatrix4fv(glGetUniformLocation(black, "modelviewMatrix"), 1, GL_TRUE, m.m);
	DrawModel(bunny, black, "inPosition", NULL, NULL);
	
	printError("display");
	
	glutSwapBuffers();
}

void keys(unsigned char key, int x, int y)
{
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (800, 800);
	glutCreateWindow ("GL3 simple shadow example");
	glutRepeatingTimerFunc(20);
	glutDisplayFunc(display); 
	glutKeyboardFunc(keys);
	init ();
	glutMainLoop();
	exit(0);
}
