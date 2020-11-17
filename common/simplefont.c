// simplefont.c, based on "font.c" from the OpenGL SDK by SGI
// Isolated to reusable code and extended by Ingemar 2009.
// New version, texture-based, OpenGL 3.2 compatible 2012.
// (No more glBitmap and no display lists.)

// This is a very simple, fixed-width bitmap font renderer.
// It is similar to the fonts in GLUT, but an isolated module,
// and a bit better! Nice new features: Placement relative to
// bottom or right (negative coordinates), smarter left edge clipping.
// font.c was CAPITAL LETTERS only, but here lower case is added, and numbers, and some more!
// It is still a simple, low-end text renderer. Try FTGL for a better one.

// Slightly modified 2013: Added "static" for internal variables and functions.
// This is important, reduces the risk of name collisions!

// Correction 2015: Sets and restores the current texture unit.

#include "simplefont.h"

static GLubyte space[] =
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// Original CAPITAL letters
GLubyte letters[][13] = {
	{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, 0x18},
	{0x00, 0x00, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},
	{0x00, 0x00, 0x7e, 0xe7, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e},
	{0x00, 0x00, 0xfc, 0xce, 0xc7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc7, 0xce, 0xfc},
	{0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xc0, 0xff},
	{0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xff},
	{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xcf, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e},
	{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
	{0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7e},
	{0x00, 0x00, 0x7c, 0xee, 0xc6, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06},
	{0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xe0, 0xf0, 0xd8, 0xcc, 0xc6, 0xc3},
	{0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0},
	{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xdb, 0xff, 0xff, 0xe7, 0xc3},
	{0x00, 0x00, 0xc7, 0xc7, 0xcf, 0xcf, 0xdf, 0xdb, 0xfb, 0xf3, 0xf3, 0xe3, 0xe3},
	{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xe7, 0x7e},
	{0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},
	{0x00, 0x00, 0x3f, 0x6e, 0xdf, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c},
	{0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},
	{0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0x7e, 0xe0, 0xc0, 0xc0, 0xe7, 0x7e},
	{0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xff},
	{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
	{0x00, 0x00, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
	{0x00, 0x00, 0xc3, 0xe7, 0xff, 0xff, 0xdb, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
	{0x00, 0x00, 0xc3, 0x66, 0x66, 0x3c, 0x3c, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3},
	{0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3},
	{0x00, 0x00, 0xff, 0xc0, 0xc0, 0x60, 0x30, 0x7e, 0x0c, 0x06, 0x03, 0x03, 0xff}};

// Lower case
static GLubyte lowletters[][13] = {
	{0x00, 0x00, 0x7d, 0xc3, 0xc3, 0xc3, 0x7f, 0x03, 0x7e, 0x00, 0x00, 0x00, 0x00}, // a
	{0x00, 0x00, 0xfe, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0}, // b
	{0x00, 0x00, 0x7f, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x7f, 0x00, 0x00, 0x00, 0x00}, // c
	{0x00, 0x00, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x7f, 0x03, 0x03, 0x03, 0x03}, // d
	{0x00, 0x00, 0x7e, 0xc0, 0xc0, 0xfe, 0xc3, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00}, // e
	{0x00, 0x00, 0x3c, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7e, 0x18, 0x18, 0x18, 0x0e}, // f
	{0x7f, 0x03, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00}, // g
	{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0}, // h
	{0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00}, // i
	{0x70, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00}, // j
	{0x00, 0x00, 0xc3, 0xc7, 0xce, 0xfc, 0xfe, 0xc7, 0xc3, 0xc0, 0xc0, 0xc0, 0xc0}, // k
	{0x00, 0x00, 0x0c, 0x1c, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18}, // l
	{0x00, 0x00, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xfe, 0x00, 0x00, 0x00, 0x00}, // m
	{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0x00, 0x00, 0x00, 0x00}, // n
	{0x00, 0x00, 0x7e, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00}, // o
	{0xc0, 0xc0, 0xfe, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0x00, 0x00, 0x00, 0x00}, // p
	{0x03, 0x03, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x7f, 0x00, 0x00, 0x00, 0x00}, // q
	{0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xe0, 0xf0, 0xdf, 0x00, 0x00, 0x00, 0x00}, // r
	{0x00, 0x00, 0xfe, 0x03, 0x03, 0x7e, 0xc0, 0xc0, 0x7f, 0x00, 0x00, 0x00, 0x00}, // s
	{0x00, 0x00, 0x0e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7e, 0x18, 0x18, 0x18, 0x18}, // t
	{0x00, 0x00, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x00, 0x00, 0x00, 0x00}, // u
	{0x00, 0x00, 0x18, 0x3c, 0x66, 0x66, 0xc3, 0xc3, 0xc3, 0x00, 0x00, 0x00, 0x00}, // v
	{0x00, 0x00, 0x66, 0x7e, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0x00, 0x00, 0x00, 0x00}, // w
	{0x00, 0x00, 0xc3, 0xe7, 0x3c, 0x18, 0x3c, 0xe7, 0xc3, 0x00, 0x00, 0x00, 0x00}, // x
	{0x7f, 0x03, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x00, 0x00, 0x00, 0x00}, // y
	{0x00, 0x00, 0xff, 0xc0, 0x70, 0x1c, 0x06, 0x03, 0xff, 0x00, 0x00, 0x00, 0x00}, // z
};

// Numbers and more (same section of ASCII)
static GLubyte numletters[][13] = {
	{0x00, 0x00, 0x3c, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3c}, // 0
	{0x00, 0x00, 0x3c, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x38, 0x18}, // 1
	{0x00, 0x00, 0x7e, 0x60, 0x60, 0x60, 0x60, 0x3c, 0x06, 0x06, 0x66, 0x66, 0x3c}, // 2
	{0x00, 0x00, 0x3c, 0x66, 0x06, 0x06, 0x06, 0x1c, 0x06, 0x06, 0x06, 0x66, 0x3c}, // 3
	{0x00, 0x00, 0x06, 0x06, 0x06, 0x06, 0x06, 0x7f, 0x66, 0x36, 0x1e, 0x0e, 0x06}, // 4
	{0x00, 0x00, 0x3c, 0x66, 0x06, 0x06, 0x06, 0x7c, 0x60, 0x60, 0x60, 0x60, 0x7e}, // 5
	{0x00, 0x00, 0x3c, 0x66, 0x66, 0x66, 0x66, 0x66, 0x7c, 0x60, 0x60, 0x66, 0x3c}, // 6
	{0x00, 0x00, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x1f, 0x06, 0x06, 0x06, 0x06, 0x7e}, // 7
	{0x00, 0x00, 0x3c, 0x66, 0x66, 0x66, 0x66, 0x3c, 0x66, 0x66, 0x66, 0x66, 0x3c}, // 8
	{0x00, 0x00, 0x3c, 0x66, 0x06, 0x06, 0x06, 0x3e, 0x66, 0x66, 0x66, 0x66, 0x3c}, // 9
	{0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00}, // :
	{0x00, 0x00, 0x30, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00}, // ;
	{0x00, 0x00, 0x06, 0x1c, 0x30, 0x60, 0x30, 0x1c, 0x06, 0x00, 0x00, 0x00, 0x00}, // <
	{0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00}, // =
	{0x00, 0x00, 0x60, 0x38, 0x0c, 0x06, 0x0c, 0x38, 0x60, 0x00, 0x00, 0x00, 0x00}, // >
	{0x00, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x0c, 0x06, 0x06, 0x66, 0x3c}, // ?
};

static GLubyte specialletters[][13] = {
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // space
	{0x00, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18}, // !
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x24, 0x00, 0x00}, // "
	{0x00, 0x00, 0x24, 0x24, 0x7e, 0x7e, 0x24, 0x7e, 0x7e, 0x24, 0x24, 0x00, 0x00}, // #
	{0x00, 0x00, 0x18, 0x3c, 0x5a, 0x5a, 0x1a, 0x3c, 0x58, 0x58, 0x5a, 0x3c, 0x18}, // $
	{0x00, 0x00, 0x44, 0x4a, 0x6a, 0x24, 0x30, 0x18, 0x0c, 0x24, 0x56, 0x52, 0x22}, // %

	{0x00, 0x00, 0x79, 0xcf, 0xc6, 0xcf, 0x79, 0x70, 0x78, 0xcc, 0xcc, 0xcc, 0x78}, // &
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x18, 0x00, 0x00}, // '
	{0x00, 0x00, 0x0c, 0x18, 0x18, 0x30, 0x30, 0x30, 0x30, 0x30, 0x18, 0x18, 0x0c}, // (
	{0x00, 0x00, 0x30, 0x18, 0x18, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x18, 0x18, 0x30}, // )
	{0x00, 0x00, 0x00, 0x00, 0x10, 0x54, 0x38, 0x54, 0x10, 0x00, 0x00, 0x00, 0x00}, // *
	{0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x7c, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00}, // +
	{0x00, 0x30, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // ,
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // -
	{0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // .
	{0x00, 0x00, 0x60, 0x60, 0x30, 0x30, 0x18, 0x18, 0x18, 0x0c, 0x0c, 0x06, 0x06}, // /
};

static GLuint fontOffset;

static int gRasterH = 800;
static int gRasterV = 600;

void sfSetRasterSize(int h, int v)
{
	gRasterH = h;
	gRasterV = v;
}

// Inline shaders
static char frag[] =
	"#version 150\n"
	"out vec4 outColor;"
	"in vec2 texCoord;"
	"uniform sampler2D tex;"
	"void main(void)"
	"{"
	"	vec4 color = texture(tex, texCoord);"
	"	if (color.r == 0.0) {"
	"		outColor = vec4(0.0, 0.0, 0.0, 0.0);"
	"	} else { "
	"		outColor = color;"
	"	}"
	"}";

static char vert[] =
	"#version 150\n"
	"in  vec3 inPosition;"
	"out vec2 texCoord;"
	"uniform int x, y, c;"
	"uniform int screenSizeX, screenSizeY;"
	"void main(void)"
	"{"
	"	int row = c / 16;"
	"	int col = c & 15;"
	"	texCoord = vec2(inPosition.x/8.0, inPosition.y/13.0);"
	"	texCoord = vec2((col + inPosition.x/8.0)/16.0, (row + (1.0 - inPosition.y/13.0))*13.0/128.0);"
	""
	"	float screenx = (x + inPosition.x)*2.0/screenSizeX - 1.0;"
	"	float screeny = -(y + inPosition.y)*2.0/screenSizeY + 1.0;"
	"	gl_Position = vec4(screenx, screeny, 0.0, 1.0);"
	"}";

// Compile a shader, return reference to it
static GLuint sfCompileShaders(const char *vs, const char *fs)
{
	GLuint v, f, p;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(v, 1, &vs, NULL);
	glShaderSource(f, 1, &fs, NULL);
	glCompileShader(v);
	glCompileShader(f);
	p = glCreateProgram();
	glAttachShader(p, v);
	glAttachShader(p, f);
	glLinkProgram(p);
	glUseProgram(p);
	return p;
}

static GLfloat vertices[] = {0.0f, 0.0f, 0.0f,
							 0.0f, 13.0f, 0.0f,
							 8.0f, 0.0f, 0.0f,

							 0.0f, 13.0f, 0.0f,
							 8.0f, 13.0f, 0.0f,
							 8.0f, 0.0f, 0.0f};

// vertex array object
static unsigned int vertexArrayObjID;
static GLuint sfProgram;

static void initVAO()
{
	unsigned int vertexBufferObjID;

	// Load and compile shader
	sfProgram = sfCompileShaders(vert, frag);
	glUseProgram(sfProgram);

	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);

	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(sfProgram, "inPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(sfProgram, "inPosition"));

	glUniform1i(glGetUniformLocation(sfProgram, "tex"), 0); // Texture unit 0
}

static void charToTexture(GLubyte *indata, int theChar, GLubyte *data)
{
	int row = theChar / 16;
	int col = theChar & 15;
	int startpos = (128 * row * 13 + col * 8) * 4;
	int i, pos, rowchar, bit;
	for (i = 0; i < 13; i++)
	{
		pos = startpos + 128 * i * 4;
		rowchar = indata[i];
		for (bit = 128; bit != 0; bit = bit >> 1)
		{
			//			printf("char %d row %d bit %d\n", theChar, i, bit);
			if ((rowchar & bit) == 0)
			{
				data[pos++] = 0;
				data[pos++] = 0;
				data[pos++] = 0;
				data[pos++] = 0;
			}
			else
			{
				data[pos++] = 255;
				data[pos++] = 255;
				data[pos++] = 255;
				data[pos++] = 255;
			}
		}
	}
}

static GLuint fontTexture;

static void fontToTexture()
{
	GLint i, j, tex;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &tex); // Save current
	GLubyte *data = malloc(128 * 128 * 4);		// 128x128 pixels, 4 bytes each
	// Why is GL_INTENSITY not available?

	// Upper case
	for (i = 0, j = 'A'; i < 26; i++, j++)
		charToTexture(letters[i], j, data);
	// Lower case
	for (i = 0, j = 'a'; i < 26; i++, j++)
		charToTexture(lowletters[i], j, data);
	// Numbers and some more
	for (i = 0, j = '0'; i < 16; i++, j++)
		charToTexture(numletters[i], j, data);
	// Space and some more
	for (i = 0, j = ' '; i < 16; i++, j++)
		charToTexture(specialletters[i], j, data);

	// ladda
	glGenTextures(1, &fontTexture); // Generate OpenGL texture IDs
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // Linear Filtered
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // Linear Filtered
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 128, 128, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glBindTexture(GL_TEXTURE_2D, tex); // Restore
}

static void drawChar(int x, int y, char c)
{
	glUseProgram(sfProgram);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	// upp med uniforms
	glUniform1i(glGetUniformLocation(sfProgram, "x"), x);
	glUniform1i(glGetUniformLocation(sfProgram, "y"), y);
	glUniform1i(glGetUniformLocation(sfProgram, "c"), c);
	glUniform1i(glGetUniformLocation(sfProgram, "screenSizeX"), gRasterH);
	glUniform1i(glGetUniformLocation(sfProgram, "screenSizeY"), gRasterV);

	glBindVertexArray(vertexArrayObjID); // Select VAO
	glDrawArrays(GL_TRIANGLES, 0, 6);	 // draw object
}

void sfMakeRasterFont(void)
{
	GLint tex;
	GLint saveprogram;

	glGetIntegerv(GL_TEXTURE_BINDING_2D, &tex);
	//	glEnable(GL_TEXTURE_2D);
	glGetIntegerv(GL_CURRENT_PROGRAM, &saveprogram);

	initVAO();

	fontToTexture();

	glBindTexture(GL_TEXTURE_2D, tex);
	glUseProgram(saveprogram);
}

// Note that negative coordinates are allowed
// and refer to the left or bottom!
void sfDrawString(int h, int v, char *s)
{
	int off;
	GLint tex;
	GLint saveprogram;
	GLint savetexunit;

	char saveZ = glIsEnabled(GL_DEPTH_TEST);
	char saveCull = glIsEnabled(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glGetIntegerv(GL_ACTIVE_TEXTURE, &savetexunit); // Save current texture unit
	glActiveTexture(GL_TEXTURE0);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &tex); // Save current texture for unit 0

	glBindTexture(GL_TEXTURE_2D, fontTexture);
	//	glEnable(GL_TEXTURE_2D); // Irrelevant
	// I also would like to get and restore the current shader program:
	glGetIntegerv(GL_CURRENT_PROGRAM, &saveprogram);

	if (h < 0)
		h = gRasterH + h - strlen(s) * 10;
	if (v < 0)
		v = gRasterV + v; // * 13;

	for (; *s != 0; s++)
	{
		drawChar(h, v - 10, *s); // -10 to get to the proper height
		h += 10;
	}
	glBindTexture(GL_TEXTURE_2D, tex);
	glActiveTexture(savetexunit);
	glUseProgram(saveprogram);
	if (saveZ)
		glEnable(GL_DEPTH_TEST);
	if (saveCull)
		glEnable(GL_CULL_FACE);

	glPopAttrib();
}
