# set this variable to the director in which you saved the common files
commondir = ../common/

l4: $(commondir)LoadTGA.c SpriteLight.c lab4.c $(commondir)VectorUtils3.c $(commondir)GL_utilities.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o lab4 -I$(commondir) $(commondir)LoadTGA.c $(commondir)VectorUtils3.c $(commondir)GL_utilities.c $(commondir)Linux/MicroGlut.c SpriteLight.c lab4.c -I../common/Linux -DGL_GLEXT_PROTOTYPES -lXt -lX11 -lGL -lm && ./lab4

l4_sep: $(commondir)LoadTGA.c SpriteLight.c lab4_sep.c $(commondir)VectorUtils3.c $(commondir)GL_utilities.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o lab4_sep -I$(commondir) $(commondir)LoadTGA.c $(commondir)VectorUtils3.c $(commondir)GL_utilities.c $(commondir)Linux/MicroGlut.c SpriteLight.c lab4_sep.c -I../common/Linux -DGL_GLEXT_PROTOTYPES -lXt -lX11 -lGL -lm && ./lab4_sep


m4 : lab4.c SpriteLight.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o lab4 -I$(commondir) -I$(commondir)/Mac	-DGL_GLEXT_PROTOTYPES lab4.c SpriteLight.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m -framework OpenGL -framework Cocoa -lm -Wno-deprecated-declarations && ./lab4

m4_sep : lab4_sep.c SpriteLight.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o lab4_sep -I$(commondir) -I$(commondir)/Mac	-DGL_GLEXT_PROTOTYPES lab4_sep.c SpriteLight.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m -framework OpenGL -framework Cocoa -lm -Wno-deprecated-declarations && ./lab4_sep


clean:
	rm -f lab4 lab4_sep
