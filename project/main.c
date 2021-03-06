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
#define kNumPrograms 12
#define stringMaxSize 32
GLuint program[kNumPrograms];
int currentProgram;
char programName[kNumPrograms][stringMaxSize] =
	{
		"phong",
		"passthrough",
		"flat",
		"wireframe",
		"balloon",
		"explode",
		"use texture",
		"normal viz",
		"grass",
		"shadow",
		"voxel try",
		"voxel", // end
};
char currentName[stringMaxSize];
// * Texture(s)
GLuint texture;

bool rotate = false;
vec3 gravity, color;

// light sources
vec3 lightSourcesColorsArr[] = {
	{1.0f, 0.0f, 0.0f}, // Red light
	{1.0f, 1.0f, 0.8f}, // Green light
	{0.0f, 0.0f, 1.0f}, // Blue light
	{1.0f, 1.0f, 1.0f}	// White light
};
GLint isDirectional[] = {1, 1, 0, 0};

vec3 lightSourcesDirectionsPositions[] = {
	{10.0f, 5.0f, 0.0f}, // Red light, positional
	{0.0f, 5.0f, 10.0f}, // Green light, positional
	{-1.0f, 0.0f, 0.0f}, // Blue light along X
	{0.0f, 0.0f, -1.0f}, // White light along Z
};

void init(void)
{
	// three vertex buffer objects, used for uploading the data
	// unsigned int normalBufferObjID;

	dumpInfo();

	// GL inits
	glClearColor(0.1, 0.1, 0.1, 0);
	glEnable(GL_BLEND);
	// glEnable(GL_DEPTH_TEST);
	// glEnable(GL_CULL_FACE);
	// glCullFace(GL_BACK);
	printError("GL inits"); // This is merely a vague indication of where something might be wrong

	projectionMatrix = perspective(100, 1.0, 0.1, 1000);
	worldToView = lookAt(0, 0, 1.5, 0, 0, 0, 0, 1, 0);
	modelToWorld = T(0, 0, -4);
	gravity = SetVector(0.0, -1.0, 1.0);
	color = SetVector(0.2, 0.9, 0.2);

	currentProgram = 0;

	// Load and compile shader
	program[0] = loadShadersG("shaders/vertex/bunny.vert", "shaders/fragment/bunny.frag", "shaders/geometry/passthrough.gs");
	program[1] = loadShadersG("shaders/vertex/minimal.vert", "shaders/fragment/uniformColor.frag", "shaders/geometry/passthrough.gs");
	program[2] = loadShadersG("shaders/vertex/minimal.vert", "shaders/fragment/minimal.frag", "shaders/geometry/flatshading.gs");
	program[3] = loadShadersG("shaders/vertex/minimal.vert", "shaders/fragment/colorIn.frag", "shaders/geometry/wireframe.gs");
	program[4] = loadShadersG("shaders/vertex/minimal.vert", "shaders/fragment/minimal.frag", "shaders/geometry/balloon.gs");
	program[5] = loadShadersG("shaders/vertex/minimal.vert", "shaders/fragment/colorShade.frag", "shaders/geometry/expand.gs");
	program[6] = loadShadersG("shaders/vertex/texture.vert", "shaders/fragment/texture.frag", "shaders/geometry/passthrough_tex.gs");
	program[7] = loadShadersG("shaders/vertex/normal.vert", "shaders/fragment/normal.frag", "shaders/geometry/normal.gs");
	program[8] = loadShadersG("shaders/vertex/normal.vert", "shaders/fragment/colorIn.frag", "shaders/geometry/fur.gs");
	program[9] = loadShadersG("shaders/vertex/minimal.vert", "shaders/fragment/shadow.frag", "shaders/geometry/duplicate.gs");
	program[10] = loadShadersG("shaders/vertex/minimal.vert", "shaders/fragment/voxel.frag", "shaders/geometry/voxel.gs");
	program[11] = loadShadersG("shaders/vertex/voxel2.vert", "shaders/fragment/voxel2.frag", "shaders/geometry/voxel2.gs");

	glUseProgram(program[currentProgram]);

	printError("init shader");

	// Upload geometry to the GPU:
	bunny = LoadModelPlus("objects/bunny.obj");
	teddy = LoadModelPlus("objects/teddy.obj");
	teapot = LoadModelPlus("objects/teapot.obj");
	cube = LoadModelPlus("objects/cubeplus.obj");
	sphere = LoadModelPlus("objects/model.obj");
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
		printError("init use program");

		glUniformMatrix4fv(glGetUniformLocation(program[i], "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
		glUniformMatrix4fv(glGetUniformLocation(program[i], "viewMatrix"), 1, GL_TRUE, worldToView.m);
		glUniform1iv(glGetUniformLocation(program[i], "index"), 4, &i);
		printError("init uniforms");

		// load textures
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(program[i], "tex"), 0); // Texture unit 0
		LoadTGATextureSimple("textures/dot.tga", &texture);
		printError("init texture");
	}
	printError("init programs");
}

void display(void)
{
	printError("pre display");
	glUseProgram(program[currentProgram]);

	// Enable Z-buffering
	glEnable(GL_DEPTH_TEST);
	// Enable backface culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ---- transforms ----
	float t = glutGet(GLUT_ELAPSED_TIME);
	GLfloat t_anim = fabs(cos(t / 1000));
	GLfloat s = 2;
	mat4 r = rotate ? Mult(Rz(t / 5000), Ry(t / -5000)) : IdentityMatrix();
	mat4 MTW = Mult(modelToWorld, S(s, s, s));
	mat4 modelToWorldToView = Mult(Mult(worldToView, MTW), r); // Combine to one matrix
	// ---- ---------- ----

	// ---- pass uniforms
	glUniform1f(glGetUniformLocation(program[currentProgram], "t"), t_anim);
	glUniformMatrix4fv(glGetUniformLocation(program[currentProgram], "modelToWorldToView"), 1, GL_TRUE, modelToWorldToView.m);
	glUniform3fv(glGetUniformLocation(program[currentProgram], "gravity"), 1, &gravity);
	glUniform3fv(glGetUniformLocation(program[currentProgram], "color"), 1, &color);
	printError("uniforms");
	// ---- ------------

	//draw the model
	DrawModel(currentModel, program[currentProgram], "inPosition", "inNormal", "in_TexCoord");
	printError("display");

	// display with normal shader
	if (currentProgram >= 7 && currentProgram < 10)
	{
		int baseprogram = 1;
		glUseProgram(program[baseprogram]);
		glUniform1f(glGetUniformLocation(program[baseprogram], "t"), t_anim);
		glUniformMatrix4fv(glGetUniformLocation(program[baseprogram], "modelToWorldToView"), 1, GL_TRUE, modelToWorldToView.m);
		DrawModel(currentModel, program[baseprogram], "inPosition", "inNormal", "in_TexCoord");
		printError("display second");
	}

	// name of the program
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
	p.y = x - prevx;
	p.x = -(prevy - y);
	p.z = 0;
	mat4 t = T(0, 0, 4);													 // move to origin
	mat4 t2 = T(0, 0, -4);													 // move to origin
	m = Mult(t2, Mult(ArbRotate(p, sqrt(p.x * p.x + p.y * p.y) / 50.0), t)); // Rotation in view coordinates
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
		currentProgram = currentProgram == kNumPrograms - 1 ? 0 : currentProgram + 1;
		printf("%d %s \n", currentProgram, currentName);
		break;
	case '-':
		currentProgram = currentProgram == 0 ? kNumPrograms - 1 : currentProgram - 1;
		printf("%d %s \n", currentProgram, currentName);
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
