# set this variable to the director in which you saved the common files
commondir = ../common/

main : main.c $(commondir)simplefont.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o main -I$(commondir) -I$(commondir)/Mac -DGL_GLEXT_PROTOTYPES main.c $(commondir)simplefont.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m -framework OpenGL -framework Cocoa -lm -Wno-deprecated-declarations && ./main

clean:
	rm -f main

