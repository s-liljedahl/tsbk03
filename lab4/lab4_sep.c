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
GLfloat kAlignmentWeight = 0;
GLfloat kCohesionWeight = 0.05;
GLfloat kAvoidanceWeight = 0.1;

GLfloat kMaxdistance = 200;
GLfloat kMindistance = 50;
GLfloat speedLimit = 2;

GLfloat limitSpeed(GLfloat speed)
{
	if (abs(speed) > speedLimit) {
		return (speed / abs(speed)) * speedLimit;
	}
	return speed;
}

void SpriteBehavior() // Din kod!
{
	SpritePtr sp;
	SpritePtr boid;
	SpritePtr updateSp;
	GLfloat speedDiffH, speedDiffV;
	GLfloat count;

	// L�gg till din labbkod h�r. Det g�r bra att �ndra var som helst i
	// koden i �vrigt, men mycket kan samlas h�r. Du kan utg� fr�n den
	// globala listroten, gSpriteRoot, f�r att kontrollera alla sprites
	// hastigheter och positioner, eller arbeta fr�n egna globaler.

	// Loop though all sprites. (Several loops in real engine.)
	sp = gSpriteRoot;
	do
	{
		count = 0;

		sp->V_align = SetVector(0,0,0);
		sp->V_avoid = SetVector(0,0,0);
		sp->P_cohesion = SetVector(0,0,0);

		// check all neighbours
		boid = gSpriteRoot;
		do
		{
			if (boid != sp)
			{ // not itself

				GLfloat diff_H = boid->position.h - sp->position.h;
				GLfloat diff_V = boid->position.v - sp->position.v;

				if (abs(diff_V) < kMaxdistance &&
						abs(diff_H) < kMaxdistance)
				{
					count++;
					// cohesion: average position
					sp->P_cohesion = VectorAdd(sp->P_cohesion, SetVector(diff_H, diff_V, 0.0));

					// align: average speed diff
					speedDiffH = sp->speed.h - boid->speed.h;
					speedDiffV = sp->speed.v - boid->speed.v;
					sp->V_align = VectorAdd(sp->V_align, SetVector(speedDiffH, speedDiffV, 0.0));


					// avoid: check for avoidable
					if (abs(diff_V) < kMindistance && abs(diff_H) < kMindistance)
					{
						sp->V_avoid = VectorSub(sp->V_avoid, SetVector(diff_H, diff_V, 0.0));
					}

				}
			}
			boid = boid->next;
		} while (boid != NULL);

		if (count > 0)
		{

			// printVec3(sp->P_cohesion);
			// printVec3(sp->V_avoid);
			// printVec3(sp->V_align);
			//
			// printFloat(Norm(sp->P_cohesion));
			// printFloat(Norm(sp->V_avoid));
			// printFloat(Norm(sp->V_align));

			if (Norm(sp->V_align) > 0) {
				sp->V_align = Normalize(ScalarMult((sp->V_align), 1.0/count));
			} else {
				sp->V_align = SetVector(0,0,0);
			}

			sp->V_avoid = (ScalarMult((sp->V_avoid), 1.0 / count));
			if (Norm(sp->V_avoid) > 0) {
				GLfloat avoid_factor  = 1 - Norm(sp->V_avoid) / kMindistance;
				sp->V_avoid = ScalarMult(Normalize(sp->V_avoid), avoid_factor);
			}

			// if (abs(sp->P_cohesion.x) < kMindistance && abs(sp->P_cohesion.y) < kMindistance) {
			// 	sp->P_cohesion = SetVector(0,0,0);
			// } else {
				sp->P_cohesion = Normalize(ScalarMult((sp->P_cohesion), 1.0 / count));
			// }

			printVec3(sp->P_cohesion);
			// printVec3(sp->V_avoid);
			// printVec3(sp->V_align);

			// printFloat(Norm(sp->P_cohesion));
			// printFloat(Norm(sp->V_avoid));
			// printFloat(Norm(sp->V_align));
			printf("\n");
		}


		sp = sp->next;
	} while (sp != NULL);



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

		// printFloat(updateSp->speed.h);
		// printVec3(updateSp->P_cohesion);
		// printVec3(updateSp->V_avoid);
		// printVec3(updateSp->V_align);
		// printFloat(speedH);
		// printFloat(limitSpeed(speedH));
		// printFloat(speedV);
		// printFloat(limitSpeed(speedV));
		// printf("end \n");

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


	// NewSprite(TextureData *f, pos h, pos v, speed hs, speed vs)

	NewSprite(sheepFace, 450, 400, 1, 1);
	NewSprite(sheepFace, 200, 100, 1.5, -1);
	NewSprite(sheepFace, 700, 400, -1, 1.5);
	NewSprite(sheepFace, 700, 0, -1, 1.5);
	NewSprite(sheepFace, 50, 50, 2, 1.5);
	NewSprite(sheepFace, 350, 50, 2, 1.5);
	NewSprite(sheepFace, 300, 50, 2, 1.5);
	NewSprite(sheepFace, 175, 90, -0.5, 1.5);
	NewSprite(dogFace, 10, 40, 2, 0);
	NewSprite(dogFace, 500, 10, -2, 0);
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
