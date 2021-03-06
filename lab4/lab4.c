// Demo of heavily simplified sprite engine
// by Ingemar Ragnemalm 2009
// used as base for lab 4 in TSBK03.
// OpenGL 3 conversion 2013.

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include "MicroGlut.h"
// uses framework Cocoa
#else
#include <GL/gl.h>
#include "MicroGlut.h"
#endif

#include <stdlib.h>
#include "LoadTGA.h"
#include "SpriteLight.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"

// L�gg till egna globaler h�r efter behov.

void SpriteBehavior() // Din kod!
{
	SpritePtr sp;
	SpritePtr boid;
	double avgPosH, avgPosV, tp_h, tp_v;
	int count;
	vec3 v;

	GLfloat kMaxdistance = 200;
	GLfloat kMindistance = 5;
	// L�gg till din labbkod h�r. Det g�r bra att �ndra var som helst i
	// koden i �vrigt, men mycket kan samlas h�r. Du kan utg� fr�n den
	// globala listroten, gSpriteRoot, f�r att kontrollera alla sprites
	// hastigheter och positioner, eller arbeta fr�n egna globaler.

	// Loop though all sprites. (Several loops in real engine.)
	sp = gSpriteRoot;
	do
	{
		// check all neighbours
		boid = gSpriteRoot;

		avgPosH = sp->position.h;
		avgPosV = sp->position.v;

		count = 1;

		do {

			if (boid != sp) { // not itself
				if(abs(boid->position.h - sp->position.h) < kMaxdistance &&
					abs(boid->position.v - sp->position.v) < kMaxdistance
				){
					// average position
					count++;
					avgPosH += boid->position.h;
					avgPosV += boid->position.v;
				}
			}
			boid = boid->next;
		} while (boid != NULL);

		if (count > 1) {
			tp_h = (avgPosH / count) - sp->position.h;
			tp_v = (avgPosV / count) - sp->position.v;
			v = Normalize(SetVector(tp_v, 0, tp_h));
			printFloat(tp_v);

			if (abs(tp_h) > kMindistance && abs(tp_v) > kMindistance) {
				sp->speed.v = v.x;
				sp->speed.h = v.z;
			} else {
				sp->speed.v = 0;
				sp->speed.h = 0;
			}
		}

		sp = sp->next;
	} while (sp != NULL);


}

// Drawing routine
void Display()
{
	SpritePtr sp;

	glClearColor(0, 0, 0.2, 1);
	glClear(GL_COLOR_BUFFER_BIT + GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	DrawBackground();

	SpriteBehavior(); // Din kod!

	// Loop though all sprites. (Several loops in real engine.)
	sp = gSpriteRoot;
	do
	{
		HandleSprite(sp); // Callback in a real engine
		DrawSprite(sp);
		sp = sp->next;
	} while (sp != NULL);

	glutSwapBuffers();
}

void Reshape(int h, int v)
{
	glViewport(0, 0, h, v);
	gWidth = h;
	gHeight = v;
}

void Timer(int value)
{
	glutTimerFunc(20, Timer, 0);
	glutPostRedisplay();
}

// Example of user controllable parameter
float someValue = 0.0;

void Key(unsigned char key,
				 __attribute__((unused)) int x,
				 __attribute__((unused)) int y)
{
	switch (key)
	{
	case '+':
		someValue += 0.1;
		printf("someValue = %f\n", someValue);
		break;
	case '-':
		someValue -= 0.1;
		printf("someValue = %f\n", someValue);
		break;
	case 0x1b:
		exit(0);
	}
}

void Init()
{
	TextureData *sheepFace, *blackFace, *dogFace, *foodFace;

	LoadTGATextureSimple("bilder/leaves.tga", &backgroundTexID); // Bakgrund

	sheepFace = GetFace("bilder/sheep.tga");	 // Ett f�r
	blackFace = GetFace("bilder/blackie.tga"); // Ett svart f�r
	dogFace = GetFace("bilder/dog.tga");			 // En hund
	foodFace = GetFace("bilder/mat.tga");			 // Mat

	NewSprite(sheepFace, 400, 400, 1, 1);
	NewSprite(sheepFace, 200, 100, 1.5, -1);
	NewSprite(sheepFace, 700, 500, -1, 1.5);
	NewSprite(sheepFace, 50, 50, 2, 1.5);
	NewSprite(sheepFace, 175, 90, -0.5, 1.5);
	NewSprite(dogFace, 20, 10, 1.5, -1);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(800, 600);
	glutInitContextVersion(3, 2);
	glutCreateWindow("SpriteLight demo / Flocking");

	glutDisplayFunc(Display);
	glutTimerFunc(20, Timer, 0); // Should match the screen synch
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Key);

	InitSpriteLight();
	Init();

	glutMainLoop();
	return 0;
}
