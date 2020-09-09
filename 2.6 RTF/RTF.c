// Recursive Temporal Filtering demo
// New edition, shader based.
// by Ingemar 2012-2013

// Note the controls.
// +/- for adjusting blend level
// b for turning bakground on/off

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

mat4 camMatrix, projectionMatrix;

Point3D camPoint = {0, 15, 10};
Point3D lookatPoint = {0, 0 ,0};
Model *m, *quadModel;
GLuint program;
GLuint tex, rtftex, tex2;
float k = 0.9; // Start value for blending

GLfloat quad[] = {
							-1,1,0.99,
							-1,-1,0.99,
							1,-1,0.99,
							1,1,0.99};
GLfloat quadTexCoord[] = {
							0,0,
							0,1,
							1,1,
							1,0};
GLuint quadIndices[] = {0, 1, 2, 0, 2, 3};

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_TRUE);

	// Load and compile shader
	program = loadShaders("rtf.vert", "rtf.frag");
	glUseProgram(program);
	
	// Upload geometry to the GPU:
	m = LoadModelPlus("teapot.obj");

	quadModel = LoadDataToModel(
			quad, NULL, quadTexCoord, NULL, quadIndices, 4, 6);
				
	// End of upload of geometry

	projectionMatrix = perspective(90, 1.0, 1.0, 1000.0);
	camMatrix = lookAt(camPoint.x, camPoint.y, camPoint.z,
								lookatPoint.x, lookatPoint.y, lookatPoint.z,
								0,1,0);

	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, camMatrix.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	glUniform1i(glGetUniformLocation(program, "rtftex"), 1); // Texture unit 1
	
	glUniform1f(glGetUniformLocation(program, "k"), k); // Blending control
	
	LoadTGATextureSimple("maskros512.tga", &tex);
	LoadTGATextureSimple("maskros512.tga", &rtftex);
	LoadTGATextureSimple("lenna256.tga", &tex2);
}

GLfloat a = 0.0;
char bg = 1;

void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// Optional background. Without it, the image will fade out
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture(GL_TEXTURE_2D, tex);
	mat4 im = IdentityMatrix();
	if (bg)
	{
		glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, im.m);
		glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, im.m);
		glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, im.m);
		DrawModel(quadModel, program, "inPosition", NULL, "inTexCoord");
	}

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glActiveTexture( GL_TEXTURE1 );
	glBindTexture(GL_TEXTURE_2D, rtftex);

	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, im.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, im.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, im.m);
	DrawModel(quadModel, program, "inPosition", NULL, "inTexCoord");

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture(GL_TEXTURE_2D, tex2);

	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, camMatrix.m); // A "look-at" function could be more practical
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

	a += 0.1;
	mat4 rot, trans, total, rot2;

	total = Mult(Mult(Ry(a/2), T(5.0, 0, 0)), Ry(M_PI/2.0));
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);

	DrawModel(m, program, "inPosition", NULL, "inTexCoord");

	glBindTexture(GL_TEXTURE_2D, rtftex);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, 512, 512);
	
	glutSwapBuffers();
}

void keydown(unsigned char key, int x, int y)
{
	switch (key)
	{
		case '+':
				k = 1.0 - (1.0 - k) * 2; // Double distance to 1
				if (k < 0.0) k = 0.0;
				glUniform1f(glGetUniformLocation(program, "k"), k); // Blending control
				printf("k = %f\n", k);
			break;
		case '-':
				k = 1.0 - (1.0 - k) / 2; // Half distance to 1
				if (k > 0.9999) k = 0.9999;
				glUniform1f(glGetUniformLocation(program, "k"), k); // Blending control
				printf("k = %f\n", k);
			break;
		case 'b':
				bg = !bg;
			break;
	}
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutInitWindowSize(512, 512);
	glutCreateWindow ("Temporal filtering example +.b");
	glutDisplayFunc(display);
	glutKeyboardFunc(keydown);
	init ();
	glutRepeatingTimerFunc(20);
	glutMainLoop();
	exit(0);
}
