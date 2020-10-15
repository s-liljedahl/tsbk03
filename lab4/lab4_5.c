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
GLfloat kAlignmentWeight = 0.005;
GLfloat kCohesionWeight = 0.05;
GLfloat kAvoidanceWeight = 0.1;

GLfloat kRandomWeight = 0.4;
GLfloat kDogWeight = 0.4;
GLfloat kDogDistance = 100;

GLfloat kMaxdistance = 150;
GLfloat kMindistance = 50;
GLfloat speedLimit = 1;

GLfloat dogPosH = 200;
GLfloat dogPosV = 200;

GLfloat limitSpeed(GLfloat speed)
{
	if (abs(speed) > speedLimit) {
		// corr direction
		return (abs(speed)/speed) * speedLimit;
	}
	return speed;
}

GLfloat calcDist(GLfloat diff1, GLfloat diff2)
{
	return sqrt(pow(diff1, 2) + pow(diff2, 2));
}

void SpriteBehavior() // Din kod!
{
	SpritePtr currentBoid;
	SpritePtr otherBoid;
	SpritePtr updateSp;
	GLfloat speedDiffH, speedDiffV;
	GLfloat count;

	// L�gg till din labbkod h�r. Det g�r bra att �ndra var som helst i
	// koden i �vrigt, men mycket kan samlas h�r. Du kan utg� fr�n den
	// globala listroten, gSpriteRoot, f�r att kontrollera alla sprites
	// hastigheter och positioner, eller arbeta fr�n egna globaler.

	// Loop though all sprites. (Several loops in real engine.)
	currentBoid = gSpriteRoot;
	do
	{
		count = 0;

		currentBoid->V_align = SetVector(0, 0, 0);
		currentBoid->V_avoid = SetVector(0, 0, 0);
		currentBoid->P_cohesion = SetVector(0, 0, 0);

		// check all neighbours
		otherBoid = gSpriteRoot;
		do
		{
			if (otherBoid != currentBoid)
			{ // not itself

				GLfloat diff_H = otherBoid->position.h - currentBoid->position.h;
				GLfloat diff_V = otherBoid->position.v - currentBoid->position.v;
				GLfloat dist = calcDist(diff_H, diff_V);

				GLfloat distDog = calcDist(dogPosH - currentBoid->position.h, dogPosV - currentBoid->position.v);
				if (distDog < kDogDistance) {
					vec3 avoid  = ScalarMult(SetVector(dogPosH - currentBoid->position.h, dogPosV - currentBoid->position.v, 0.0), kDogWeight);
					GLfloat avoid_factor = (1 - Norm(avoid) / kDogDistance);
					currentBoid->V_avoid = VectorSub(currentBoid->V_avoid, ScalarMult(avoid, avoid_factor));
				}
				else if (dist < kMaxdistance)
				{
					count++;
					// cohesion: average position
					currentBoid->P_cohesion = VectorAdd(currentBoid->P_cohesion, SetVector(diff_H, diff_V, 0.0));
					// align: average speed diff
					speedDiffH = otherBoid->speed.h - currentBoid->speed.h;
					speedDiffV = otherBoid->speed.v - currentBoid->speed.v;
					currentBoid->V_align = VectorAdd(currentBoid->V_align, SetVector(speedDiffH, speedDiffV, 0.0));

					// avoid: check for avoidable
					if (dist < kMindistance)
					{
						currentBoid->V_avoid = VectorSub(currentBoid->V_avoid, SetVector(diff_H, diff_V, 0.0));
					}
				}
			}
			otherBoid = otherBoid->next;
		} while (otherBoid != NULL);

		if (count > 0)
		{
			currentBoid->P_cohesion = Normalize(ScalarMult((currentBoid->P_cohesion), 1.0 / count));

			if (Norm(currentBoid->V_align) > 0)
			{
				currentBoid->V_align = Normalize(ScalarMult((currentBoid->V_align), 1.0 / count));
			}someValue
			else
			{
				currentBoid->V_align = SetVector(0, 0, 0);
			}

			currentBoid->V_avoid = ScalarMult((currentBoid->V_avoid), 1.0 / count);
			if (Norm(currentBoid->V_avoid) > 0)
			{
				GLfloat avoid_factor = 1 - Norm(currentBoid->V_avoid) / kMindistance;
				currentBoid->V_avoid = ScalarMult(Normalize(currentBoid->V_avoid), avoid_factor);
			}
		}

		currentBoid = currentBoid->next;
	} while (currentBoid != NULL);

	// second loop
	updateSp = gSpriteRoot;
	do
	{
		GLfloat speedH =
				updateSp->V_align.x * kAlignmentWeight +
				updateSp->V_avoid.x * kAvoidanceWeight +
				updateSp->P_cohesion.x * kCohesionWeight;

		GLfloat speedV =
				updateSp->V_align.y * kAlignmentWeight +
				updateSp->V_avoid.y * kAvoidanceWeight +
				updateSp->P_cohesion.y * kCohesionWeight;


		if (updateSp->isRandom)
		{
			GLfloat random_move = (((float)rand() / (float)RAND_MAX - 0.5) * kRandomWeight);
			// updateSp->speed.h += random_move;
			// updateSp->speed.v += random_move;
			speedH += random_move;
			speedV += random_move;
			// printFloat(random_move);
		}

		updateSp->speed.h += (speedH);
		updateSp->speed.v += (speedV);

		updateSp->speed.h = limitSpeed(updateSp->speed.h);
		updateSp->speed.v = limitSpeed(updateSp->speed.v);


		updateSp = updateSp->next;
	} while (updateSp != NULL);
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

	DrawDog(dogPosH, dogPosV);
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
		kDogWeight += 0.1;
		printf("someValue = %f\n", kDogWeight);
		break;
	case '-':
		kDogWeight -= 0.1;
		printf("someValue = %f\n", kDogWeight);
		break;
	case 'w':
		dogPosV += 10;
		break;
	case 'a':
		dogPosH -= 10;
		break;
	case 's':
		dogPosV -= 10;
		break;
	case 'd':
		dogPosH += 10;
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

	// NewSprite(TextureData *f, pos h, pos v, speed hs, speed vs)

	for (int n = 0; n < 10; n++)
	{
		NewSprite(sheepFace,
							rand() % 700 + 50,
							rand() % 500 + 50,
							-2,
							0, false);
	}

	NewSprite(blackFace, 0, 0, 1, 1, true);
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
