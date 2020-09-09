// Robot arm demo
// Demo of inverse kinematics. We move in a direction where we get
// closer to the desired position, a kind of hill climbing/gradient descent.
// By Ingemar spring 2019. Cleaned up to be a decent demo october 2019.

// MS Windows needs GLEW or glee.
#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// linking hint for Lightweight IDE
	//uses framework Cocoa
#endif
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

mat4 projectionMatrix;

Model *bunny, *floormodel, *sphere, *cylinder, *claw;
GLuint grasstex;

GLuint skyboxTexture;
Model *skyboxModel;

// Floor quad
GLfloat vertices2[] = {	-20.5,0.0,-20.5,
						20.5,0.0,-20.5,
						20.5,0.0,20.5,
						-20.5,0.0,20.5};
GLfloat texcoord2[] = {	100.0f, 100.0f,
						0.0f, 100.0f,
						0.0f, 0.0f,
						100.0f, 0.0f};
GLuint indices2[] = {	0,3,2, 0,2,1};

// Reference to shader programs
GLuint phongShader, texShader;

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	printError("GL inits");

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 300.0);

	// Load and compile shader
	texShader = loadShaders("textured.vert", "textured.frag");
	phongShader = loadShaders("phong.vert", "phong.frag");
	printError("init shader");
	
	// Upload geometry to the GPU:
	floormodel = LoadDataToModel(vertices2, NULL, texcoord2, NULL,
			indices2, 4, 6);
			
	bunny = LoadModelPlus("bunnyplus.obj");
	sphere = LoadModelPlus("sphere.obj");
	cylinder = LoadModelPlus("cylinder.obj");
	claw = LoadModelPlus("claw.obj");

// Important! The shader we upload to must be active!
	glUseProgram(phongShader);
	glUniformMatrix4fv(glGetUniformLocation(phongShader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUseProgram(texShader);
	glUniformMatrix4fv(glGetUniformLocation(texShader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);

	glUniform1i(glGetUniformLocation(texShader, "tex"), 0); // Texture unit 0

	LoadTGATextureSimple("grass.tga", &grasstex);
	glBindTexture(GL_TEXTURE_2D, grasstex);
	glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_WRAP_S,	GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_WRAP_T,	GL_REPEAT);

	LoadTGATextureSimple("SkyBox512.tga", &skyboxTexture);
	skyboxModel = LoadModelPlus("skybox.obj");

	printError("init arrays");
}

GLfloat a = 1.0;
vec3 campos = vec3(0, 3.0, 8);
vec3 forward = vec3(0, 0, -4);
vec3 up = vec3(0, 1, 0);
int cam = 1;
char ik = 0; // inverse kinematics


// ROBOT ARM ROTATIONS:
float r0 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0;
vec3 bunnypos = vec3(4, 4, 0);


void DrawSomething(Model *mdl, GLuint shader, mat4 m)
{
	glUseProgram(shader);
	glUniformMatrix4fv(glGetUniformLocation(shader, "modelviewMatrix"), 1, GL_TRUE, m.m);
	DrawModel(mdl, shader, "inPosition", "inNormal", "inTexCoord");
}

mat4 worldToView;

vec3 ClawPosition(GLfloat r0, GLfloat r1, GLfloat r2, GLfloat r3, GLfloat r4)
{
	mat4 rot0 = Ry(r0);
	mat4 rot1 = Rz(r1);
	mat4 trans1 = T(0,2,0);
	mat4 rot2 = Rz(r2);
	mat4 trans2 = T(0,2,0);
	mat4 rot3 = Rz(r3);
	mat4 rot4 = Ry(r4);
	mat4 trans3 = T(0,2,0);

	mat4 m = /* worldToView * */ rot0 * rot1 * trans1 * rot2 * trans2 * rot3 * trans3;
	vec3 p = vec3(0,0,0);
	
	p = m * p;
	return p;
}


void display(void)
{
	int i, j;
	
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	mat4 m; // m1, m2, m, tr, scale;
	
	if (glutKeyIsDown('a'))
		forward = MultMat3Vec3(mat4tomat3(Ry(0.01)), forward);
	if (glutKeyIsDown('d'))
		forward = MultMat3Vec3(mat4tomat3(Ry(-0.01)), forward);
	if (glutKeyIsDown('w'))
		campos = VectorAdd(campos, ScalarMult(forward, 0.01));
	if (glutKeyIsDown('s'))
		campos = VectorSub(campos, ScalarMult(forward, 0.01));
	if (glutKeyIsDown('q'))
	{
		vec3 side = CrossProduct(forward, SetVector(0,1,0));
		campos = VectorSub(campos, ScalarMult(side, 0.01));
	}
	if (glutKeyIsDown('e'))
	{
		vec3 side = CrossProduct(forward, SetVector(0,1,0));
		campos = VectorAdd(campos, ScalarMult(side, 0.01));
	}
	
	// Move up/down
	if (glutKeyIsDown('z'))
		campos = VectorAdd(campos, ScalarMult(SetVector(0,1,0), 0.01));
	if (glutKeyIsDown('c'))
		campos = VectorSub(campos, ScalarMult(SetVector(0,1,0), 0.01));
	// Rotate up/down (WRONG)
	if (glutKeyIsDown('+'))
	{
		forward = MultMat3Vec3(mat4tomat3(Rx(0.01)), forward);
	}
	if (glutKeyIsDown('-'))
	{
		forward = MultMat3Vec3(mat4tomat3(Rx(-0.01)), forward);
	}


	// Move bunny
	if (glutKeyIsDown(GLUT_KEY_LEFT))
		bunnypos.x -= 0.05;
	if (glutKeyIsDown(GLUT_KEY_RIGHT))
		bunnypos.x += 0.05;
	if (glutKeyIsDown(GLUT_KEY_UP))
		bunnypos.y += 0.05;
	if (glutKeyIsDown(GLUT_KEY_DOWN))
		bunnypos.y -= 0.05;
	if (glutKeyIsDown(GLUT_KEY_COMMA))
		bunnypos.z -= 0.05;
	if (glutKeyIsDown(GLUT_KEY_DECIMAL))
		bunnypos.z += 0.05;

	// Manually move robot arm
	// r1 r2 r3
	if (glutKeyIsDown('1'))
		r0 -= 0.05;
	if (glutKeyIsDown('2'))
		r0 += 0.05;
	if (glutKeyIsDown('3'))
		r1 -= 0.05;
	if (glutKeyIsDown('4'))
		r1 += 0.05;
	if (glutKeyIsDown('5'))
		r2 -= 0.05;
	if (glutKeyIsDown('6'))
		r2 += 0.05;
	if (glutKeyIsDown('7'))
		r3 -= 0.05;
	if (glutKeyIsDown('8'))
		r3 += 0.05;
	if (glutKeyIsDown('9'))
		r4 -= 0.05;
	if (glutKeyIsDown('0'))
		r4 += 0.05;
	
	a += 0.01;
	
	if (ik)
	{
		float diff = 0.01;
		vec3 cp = ClawPosition(r0, r1, r2, r3, r4);
		vec3 cpd0 = ClawPosition(r0+diff, r1, r2, r3, r4); // Claw position if r0 is changed
		vec3 cpd1 = ClawPosition(r0, r1+diff, r2, r3, r4);
		vec3 cpd2 = ClawPosition(r0, r1, r2+diff, r3, r4);
		vec3 cpd3 = ClawPosition(r0, r1, r2, r3+diff, r4);
		vec3 cpd4 = ClawPosition(r0, r1, r2, r3, r4+diff);
	// cpd 0-4 - bpd is a matrix! Describes the change for xyz for each set of angles.
	// Did distance to bunny improve?
		float tr0 = r0 + Norm(bunnypos - cp) - Norm(bunnypos - cpd0);
		float tr1 = r1 + Norm(bunnypos - cp) - Norm(bunnypos - cpd1);
		float tr2 = r2 + Norm(bunnypos - cp) - Norm(bunnypos - cpd2);
		float tr3 = r3 + Norm(bunnypos - cp) - Norm(bunnypos - cpd3);
		float tr4 = r4;
		
		vec3 nc = ClawPosition(tr0, tr1, tr2, tr3, tr4);
		if (Norm(bunnypos - nc) < Norm(bunnypos - cp))
		{
			r0 = tr0;
			r1 = tr1;
			r2 = tr2;
			r3 = tr3;
			r4 = tr4;
		}
	// r4 does not affect the position!
	}

	worldToView = lookAtv(campos, VectorAdd(campos, forward), up); // Manual

	//SKYBOX:
	glUseProgram(texShader); // Important!
	// Copy for the skybox
	mat4 skyCamMatrix = worldToView;
	// Zero translation
	skyCamMatrix.m[3] = 0.0f;
	skyCamMatrix.m[7] = 0.0f;
	skyCamMatrix.m[11] =0.0f;
	
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glUniformMatrix4fv(glGetUniformLocation(texShader, "modelviewMatrix"), 1, GL_TRUE, skyCamMatrix.m);
	glBindTexture(GL_TEXTURE_2D, skyboxTexture);
	DrawModel(skyboxModel, texShader, "inPosition", NULL, "inTexCoord");
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Floor
	glBindTexture(GL_TEXTURE_2D, grasstex);
	glUseProgram(texShader);
	m = worldToView;
	glUniformMatrix4fv(glGetUniformLocation(texShader, "modelviewMatrix"), 1, GL_TRUE, m.m);
	DrawModel(floormodel, texShader, "inPosition", NULL, "inTexCoord");
	
	
	// SCENE CONTENT

	// Root sphere
	m = worldToView;
	DrawSomething(sphere, phongShader, m);
	
	// First section
	mat4 rot0 = Ry(r0);
	mat4 rot1 = Rz(r1);
	m = worldToView * rot0 * rot1;
	DrawSomething(cylinder, phongShader, m);
	
	// Second sphere
	mat4 trans1 = T(0,2,0);
	m = worldToView * rot0 * rot1 * trans1;
	DrawSomething(sphere, phongShader, m);

	// Second section
	mat4 rot2 = Rz(r2);
	m = worldToView * rot0 * rot1 * trans1 * rot2;
	DrawSomething(cylinder, phongShader, m);
	
	// Third sphere
	mat4 trans2 = T(0,2,0);
	m = worldToView * rot0 * rot1 * trans1 * rot2 * trans2;
	DrawSomething(sphere, phongShader, m);

	// Third section
	mat4 rot3 = Rz(r3);
	m = worldToView * rot0 * rot1 * trans1 * rot2 * trans2 * rot3;
	DrawSomething(cylinder, phongShader, m);

	// Third sphere
	mat4 rot4 = Ry(r4);
	mat4 trans3 = T(0,2,0);
	m = worldToView * rot0 * rot1 * trans1 * rot2 * trans2 * rot3 * trans3 * rot4;
	DrawSomething(claw, phongShader, m);


	glUseProgram(phongShader);
	m = worldToView * T(bunnypos.x, bunnypos.y, bunnypos.z);
	glUniformMatrix4fv(glGetUniformLocation(phongShader, "modelviewMatrix"), 1, GL_TRUE, m.m);
	DrawModel(bunny, phongShader, "inPosition", "inNormal", "inTexCoord");

	
	glutSwapBuffers();
}

void keys(unsigned char key, int x, int y)
{
	switch (key)
	{
		case '1': cam = 1; break;
		case '2': cam = 2; break;
		case '3': cam = 3; break;
		case '4': cam = 4; break;
	}
	if (key == ' ')
		ik = !ik;
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("GL3 Robotarm");
	glutRepeatingTimerFunc(20);
	glutDisplayFunc(display); 
	glutKeyboardFunc(keys);
	init ();
	glutMainLoop();
}
