# set this variable to the director in which you saved the common files
commondir = ../common/

all: $(commondir)LoadTGA.c SpriteLight.c main.c $(commondir)VectorUtils3.c $(commondir)GL_utilities.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o main -I$(commondir) $(commondir)LoadTGA.c $(commondir)VectorUtils3.c $(commondir)GL_utilities.c $(commondir)Linux/MicroGlut.c SpriteLight.c main.c -I../common/Linux -DGL_GLEXT_PROTOTYPES -lXt -lX11 -lGL -lm

main : main.c SpriteLight.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o main -I$(commondir) -I$(commondir)/Mac	-DGL_GLEXT_PROTOTYPES main.c SpriteLight.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m -framework OpenGL -framework Cocoa -lm -Wno-deprecated-declarations

clean:
	rm -f main

