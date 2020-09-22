# set this variable to the director in which you saved the common files
commondir = ../common/

all : m2-1 m2-1-soft m2-1-gpu m2-2

lab2-1 : skinning.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o skinning -I$(commondir) -I$(commondir)/Linux -DGL_GLEXT_PROTOTYPES skinning.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm

m2-1   : skinning.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o skinning -I$(commondir) -I$(commondir)/Mac   -DGL_GLEXT_PROTOTYPES skinning.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m -framework OpenGL -framework Cocoa -lm -Wno-deprecated-declarations

m2-1-soft   : skinning_soft.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o skinning_soft -I$(commondir) -I$(commondir)/Mac   -DGL_GLEXT_PROTOTYPES skinning_soft.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m -framework OpenGL -framework Cocoa -lm -Wno-deprecated-declarations

m2-1-gpu   : skinning_soft_gpu.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o skinning_soft_gpu -I$(commondir) -I$(commondir)/Mac   -DGL_GLEXT_PROTOTYPES skinning_soft_gpu.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m -framework OpenGL -framework Cocoa -lm -Wno-deprecated-declarations

lab2-2 : skinning2.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o skinning2 -I$(commondir) -I$(commondir)/Linux -DGL_GLEXT_PROTOTYPES skinning2.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm

m2-2   : skinning2.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o skinning2 -I$(commondir) -I$(commondir)/Mac   -DGL_GLEXT_PROTOTYPES skinning2.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m -framework OpenGL -framework Cocoa -lm -Wno-deprecated-declarations

clean :
	rm skinning skinning2
	