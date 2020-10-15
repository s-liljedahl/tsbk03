// SpriteLight - Heavily simplified sprite engine
// by Ingemar Ragnemalm 2009

// What does a mogwai say when it sees a can of soda?
// Eeek! Sprite light! Sprite light!

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include "MicroGlut.h"
#else
#include <GL/gl.h>
#include "MicroGlut.h"
#endif

#include "LoadTGA.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"

typedef struct FPoint
{
	GLfloat h, v;
} FPoint;

typedef struct SpriteRec
{
	FPoint position;
	TextureData *face;
	FPoint speed;
	GLfloat rotation;
	bool isRandom;
	vec3 V_avoid;
	vec3 V_align;
	vec3 P_cohesion;

	struct SpriteRec *next;

	// Add custom sprite data here as needed
} SpriteRec, *SpritePtr;

// Globals: The sprite list, background texture and viewport dimensions (virtual or real pixels)
extern SpritePtr gSpriteRoot;
extern GLuint backgroundTexID;
extern long gWidth, gHeight;

// Functions
TextureData *GetFace(char *fileName);
struct SpriteRec *NewSprite(TextureData *f, GLfloat h, GLfloat v, GLfloat hs, GLfloat vs, bool isRandom);
void HandleSprite(SpritePtr sp);
void DrawSprite(SpritePtr sp);
void DrawBackground();

void InitSpriteLight();
