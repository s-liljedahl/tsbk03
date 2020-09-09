UNAME := $(shell uname)
os = ''
file = ''

ifeq ($(UNAME), $(os))
# do something $(os)-y
	os = Linux
	file = /MicroGlut.c
endif
ifeq ($(UNAME), Darwin)
	os = Mac
	file = /MicroGlut.m
endif

# set this variable to the director in which you saved the common files
commondir = ../common/

all : lab0

lab0: lab0.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)$(os)$(file)
	gcc -Wall -o lab0 -I$(commondir) -I../common/$(os) -DGL_GLEXT_PROTOTYPES lab0.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)$(os)$(file) -framework Cocoa -framework OpenGL -w

mac: gcc lab0.c ../common/*.c ../common/Mac/MicroGlut.m -o lab0 -framework OpenGL -framework Cocoa -I../common/Mac -I../common

clean :
	rm lab0

