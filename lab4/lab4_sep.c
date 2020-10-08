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
GLfloat kAlignmentWeight = 0.1;
GLfloat kCohesionWeight = 0.7;
GLfloat kAvoidanceWeight = 1;
GLfloat kMaxdistance = 100;
GLfloat kMindistance = 5;
GLfloat speedLimit = 2;

GLfloat limitSpeed(GLfloat speed)
{
	return (abs(speed) > speedLimit) ? (speed / abs(speed)) * speedLimit
																	 : speed;
}

void SpriteBehavior() // Din kod!
{
	SpritePtr sp;
	SpritePtr boid;
	double avgPosH, avgPosV, tp_h, tp_v, avgSpeedV, avgSpeedH;
	int count;
	vec3 avoidance, avgPos;
	vec3 v_cohesion, v_avoid, v_align;

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

		avgPos = SetVector(sp->position.v, sp->position.h, 0);
		avoidance = SetVector(0, 0, 0);

		count = 1;

		do
		{
			if (boid != sp)
			{ // not itself

				GLfloat diff_V = boid->position.v - sp->position.v;
				GLfloat diff_H = boid->position.h - sp->position.h;

				if (abs(diff_V) < kMaxdistance &&
						abs(diff_H) < kMaxdistance)
				{
					// average position
					avgPosH += boid->position.h;
					avgPosV += boid->position.v;

					// average speed diff
					avgSpeedV += sp->speed.v - boid->speed.v;
					avgSpeedH += sp->speed.h - boid->speed.h;

					// check for avoidable
					if (abs(diff_V) < kMindistance && abs(diff_H) < kMindistance)
					{
						avoidance.x -= diff_V;
						avoidance.y -= diff_H;
					}

					count += 1;
				}
			}
			boid = boid->next;
		} while (boid != NULL);

		if (count > 1)
		{
			tp_h = (avgPosH / count) - sp->position.h;
			tp_v = (avgPosV / count) - sp->position.v;

			v_align = Normalize(SetVector(avgSpeedV / (count - 1), avgSpeedH / (count - 1), 0));
			v_avoid = (Norm(avoidance) > 0) ? Normalize(SetVector(avoidance.x / count, avoidance.y / count, 0)) : SetVector(0, 0, 0);
			v_cohesion = (tp_v > kMindistance && tp_h > kMindistance) ? Normalize(SetVector(tp_v, tp_h, 0)) : SetVector(0, 0, 0);

			sp->speed.v = limitSpeed(sp->speed.v + (v_cohesion.x * kCohesionWeight) + (v_avoid.x * kAvoidanceWeight) + (v_align.x * kAlignmentWeight));
			sp->speed.h = limitSpeed(sp->speed.h + (v_cohesion.y * kCohesionWeight) + (v_avoid.y * kAvoidanceWeight) + (v_align.y * kAlignmentWeight));
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
	NewSprite(sheepFace, 700, 400, -1, 1.5);
	NewSprite(sheepFace, 700, 0, -1, 1.5);
	NewSprite(sheepFace, 50, 50, 2, 1.5);
	NewSprite(sheepFace, 350, 50, 2, 1.5);
	NewSprite(sheepFace, 350, 50, 2, 1.5);
	NewSprite(sheepFace, 175, 90, -0.5, 1.5);
	NewSprite(dogFace, 20, 10, 1.5, -1);
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
