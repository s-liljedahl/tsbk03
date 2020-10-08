# set this variable to the director in which you saved the common files
commondir = ../common/

all: $(commondir)LoadTGA.c SpriteLight.c lab4.c $(commondir)VectorUtils3.c $(commondir)GL_utilities.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o lab4 -I$(commondir) $(commondir)LoadTGA.c $(commondir)VectorUtils3.c $(commondir)GL_utilities.c $(commondir)Linux/MicroGlut.c SpriteLight.c lab4.c -I../common/Linux -DGL_GLEXT_PROTOTYPES -lXt -lX11 -lGL -lm

m4 : lab4.c SpriteLight.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o lab4 -I$(commondir) -I$(commondir)/Mac	-DGL_GLEXT_PROTOTYPES lab4.c SpriteLight.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m -framework OpenGL -framework Cocoa -lm -Wno-deprecated-declarations

m4_sep : lab4_sep.c SpriteLight.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o lab4_sep -I$(commondir) -I$(commondir)/Mac	-DGL_GLEXT_PROTOTYPES lab4_sep.c SpriteLight.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m -framework OpenGL -framework Cocoa -lm -Wno-deprecated-declarations


clean:
	rm -f lab4 lab4_sep

