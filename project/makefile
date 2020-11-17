# set this variable to the director in which you saved the common files
commondir = ../common/

stanford : stanford.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o stanford -I$(commondir) -I$(commondir)/Mac	-DGL_GLEXT_PROTOTYPES stanford.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m -framework OpenGL -framework Cocoa -lm -Wno-deprecated-declarations

main : main.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o main -I$(commondir) -I$(commondir)/Mac	-DGL_GLEXT_PROTOTYPES main.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m -framework OpenGL -framework Cocoa -lm -Wno-deprecated-declarations && ./main

bunny : bunny.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o bunny -I$(commondir) -I$(commondir)/Mac	-DGL_GLEXT_PROTOTYPES bunny.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m -framework OpenGL -framework Cocoa -lm -Wno-deprecated-declarations && ./bunny

clean:
	rm -f stanford main

