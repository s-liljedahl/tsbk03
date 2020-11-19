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
#include "simplefont.h"

//constants
const int initWidth = 512, initHeight = 512;

// Model-to-world matrix
// Modify this matrix.
// See below for how it is applied to your model.
mat4 modelToWorld;
// World-to-view matrix. Usually set by lookAt() or similar.
mat4 worldToView;
// Projection matrix, set by a call to perspective().
mat4 projectionMatrix;

// Globals
// * Model(s)
Model *bunny, *sphere, *teddy, *teapot, *cube;
Model *currentModel;

// * Reference(s) to shader program(s)
#define kNumPrograms 7
#define stringMaxSize 32
GLuint program[kNumPrograms];
int currentProgram;
char programName[kNumPrograms][stringMaxSize] = {"phong + passthrough", "passthrough", "flat", "balloon", "expand", "texture", "grass"};
char currentName[stringMaxSize];
// * Texture(s)
GLuint texture;

bool rotate = false;

// light sources
vec3 lightSourcesColorsArr[] = {
	{1.0f, 0.0f, 0.0f}, // Red light
	{0.0f, 0.7f, 0.5f}, // Green light
	{0.0f, 0.0f, 1.0f}, // Blue light
	{1.0f, 1.0f, 1.0f}	// White light
};
GLint isDirectional[] = {1, 1, 1, 1};

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
	// unsigned int normalBufferObjID;

	dumpInfo();

	// GL inits
	glClearColor(0.7, 0.6, 0.0, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	printError("GL inits"); // This is merely a vague indication of where something might be wrong

	projectionMatrix = perspective(100, 1.0, 0.1, 1000);
	worldToView = lookAt(0, 0, 1.5, 0, 0, 0, 0, 1, 0);
	modelToWorld = IdentityMatrix();

	currentProgram = 0;

	// Load and compile shader
	program[0] = loadShadersG("shaders/bunny.vert", "shaders/bunny.frag", "shaders/passthrough.gs");
	program[1] = loadShadersG("shaders/minimal.vert", "shaders/minimal.frag", "shaders/passthrough.gs");
	program[2] = loadShadersG("shaders/minimal.vert", "shaders/minimal.frag", "shaders/flatshading.gs");
	program[3] = loadShadersG("shaders/minimal.vert", "shaders/minimal.frag", "shaders/balloon.gs");
	program[4] = loadShadersG("shaders/minimal.vert", "shaders/minimal.frag", "shaders/expand.gs");
	program[5] = loadShadersG("shaders/texture.vert", "shaders/texture.frag", "shaders/passthrough_tex.gs");
	program[6] = loadShadersG("shaders/minimal.vert", "shaders/minimal.frag", "shaders/grass.gs");

	glUseProgram(program[currentProgram]);
	printError("init shader");

	// Upload geometry to the GPU:
	bunny = LoadModelPlus("objects/bunny.obj");
	teddy = LoadModelPlus("objects/teddy.obj");
	teapot = LoadModelPlus("objects/teapot.obj");
	cube = LoadModelPlus("objects/cubeplus.obj");
	sphere = LoadModelPlus("objects/groundsphere.obj");
	currentModel = bunny;
	printError("load models");

	glUseProgram(program[0]);
	glUniform3fv(glGetUniformLocation(program[0], "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
	glUniform3fv(glGetUniformLocation(program[0], "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
	glUniform1iv(glGetUniformLocation(program[0], "isDirectional"), 4, isDirectional);
	printError("init 0");

	glActiveTexture(GL_TEXTURE0);

	for (int i = 0; i < kNumPrograms; i++)
	{
		glUseProgram(program[i]);

		glUniformMatrix4fv(glGetUniformLocation(program[i], "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
		glUniform1iv(glGetUniformLocation(program[i], "index"), 4, &i);

		// load textures
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(program[i], "tex"), 0); // Texture unit 0
		LoadTGATextureSimple("objects/scale.tga", &texture);
	}
	printError("init programs");
}

void display(void)
{
	printError("pre display");
	glUseProgram(program[currentProgram]);

	float t = glutGet(GLUT_ELAPSED_TIME);

	// // Enable Z-buffering
	// glEnable(GL_DEPTH_TEST);
	// // Enable backface culling
	// glEnable(GL_CULL_FACE);
	// glCullFace(GL_BACK);

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat t_anim = fabs(cos(t / 1000));
	GLfloat s = currentProgram == 3 ? (0.8 + (t_anim / 10)) : 0.8;

	mat4 r = rotate ? Mult(Rz(t / 5000), Ry(t / -5000)) : IdentityMatrix();
	mat4 MTW = Mult(modelToWorld, S(s, s, s));
	mat4 modelToWorldToView = Mult(Mult(worldToView, MTW), r); // Combine to one matrix

	glUniform1f(glGetUniformLocation(program[currentProgram], "t"), t_anim);
	glUniformMatrix4fv(glGetUniformLocation(program[currentProgram], "modelToWorldToView"), 1, GL_TRUE, modelToWorldToView.m);
	glUniform1f(glGetUniformLocation(program[currentProgram], "specularExponent"), specularExponent[0]);

	//draw the model
	DrawModel(currentModel, program[currentProgram], "inPosition", "inNormal", "in_TexCoord");
	printError("display");

	strcpy(currentName, programName[currentProgram]);
	sfDrawString(20, 20, currentName);
	printError("display text ");

	glutSwapBuffers();
}

// Trackball
int prevx = 0, prevy = 0;

void mouseDragged(int x, int y)
{
	vec3 p;
	mat4 m;
	// This is a simple and IMHO really nice trackball system:
	// Use the movement direction to create an orthogonal rotation axis
	p.y = x - prevx;
	p.x = -(prevy - y);
	p.z = 0;
	// Create a rotation around this axis and premultiply it on the model-to-world matrix
	// Limited to fixed camera! Will be wrong if the camera is moved!
	m = ArbRotate(p, sqrt(p.x * p.x + p.y * p.y) / 50.0); // Rotation in view coordinates
	modelToWorld = Mult(m, modelToWorld);
	prevx = x;
	prevy = y;
	glutPostRedisplay();
}

void mouseUpDown(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		prevx = x;
		prevy = y;
	}
}

void Key(unsigned char key)
{
	switch (key)
	{
	case '+':
		printf("%d %s \n", currentProgram, currentName);
		currentProgram = currentProgram == kNumPrograms - 1 ? 0 : currentProgram + 1;
		break;
	case '-':
		printf("%d %s \n", currentProgram, currentName);
		currentProgram = currentProgram == 0 ? kNumPrograms - 1 : currentProgram - 1;
		break;
	case '1':
		printf("Bunny \n");
		currentModel = bunny;
		break;
	case '2':
		printf("Teddy \n");
		currentModel = teddy;
		break;
	case '3':
		printf("Teapot \n");
		currentModel = teapot;
		break;
	case '4':
		printf("Cube \n");
		currentModel = cube;
		break;
	case '5':
		printf("Sphere \n");
		currentModel = sphere;
		break;
	case 'r':
		printf("Rotate \n");
		rotate = !rotate;
		break;
	case 0x1b:
		exit(0);
	}
}

void Timer(int value)
{
	glutTimerFunc(20, Timer, 0);
	glutPostRedisplay();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitContextVersion(3, 2);
	glutInitWindowSize(700, 700);
	glutCreateWindow("Project");
	glutDisplayFunc(display);
	glutRepeatingTimer(20);
	glutTimerFunc(20, Timer, 0); // Should match the screen synch
	glutMouseFunc(mouseUpDown);
	glutMotionFunc(mouseDragged);
	glutKeyboardFunc(Key);

	sfMakeRasterFont(); // init font
	sfSetRasterSize(700, 700);

	init();
	glutMainLoop();
	exit(0);
}
