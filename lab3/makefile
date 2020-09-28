# set this variable to the director in which you saved the common files
commondir = ../common/

all : lab3 lab3-2
mac: m3 m3-2

lab3 : lab3.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)zpr.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o lab3 -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES lab3.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)zpr.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm

lab3-2 : lab3-2.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)zpr.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o lab3-2 -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES lab3-2.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)zpr.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm

m3 : lab3.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o lab3 -I$(commondir) -I$(commondir)/Mac	-DGL_GLEXT_PROTOTYPES lab3.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m -framework OpenGL -framework Cocoa -lm -Wno-deprecated-declarations

m3-2 : lab3-2.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o lab3-2 -I$(commondir) -I$(commondir)/Mac	-DGL_GLEXT_PROTOTYPES lab3-2.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m -framework OpenGL -framework Cocoa -lm -Wno-deprecated-declarations

clean :
	rm lab3 lab3-2
