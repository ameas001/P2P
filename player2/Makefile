## CS134 Spring 2014: Video Game Design
## Lab 2 Makefile
##
## This file does not need to be modified
#########################################

LIBS = -lglut -lGLU -lGL -lXmu -lXext -lXi -lX11 -lm
CC = g++

## Global header files
INCLUDE = const.h

## Object files and executables
MAIN_OUT = a.out
QUADTREE_OUT = quadtree.o
OBJECT_OUT = object.o

## Requirements for each command
MAIN_REQS = main.cpp $(OBJECT_OUT) $(QUADTREE_OUT)
QUADTREE_REQS = quadtree.h quadtree.cpp $(OBJECT_OUT)
OBJECT_REQS = object.h object.cpp

## Targets to compile for each command
MAIN_TARGETS = main.cpp $(OBJECT_OUT) $(QUADTREE_OUT)
QUADTREE_TARGETS = quadtree.cpp
OBJECT_TARGETS = object.cpp

all: main

## Main 
main: $(MAIN_REQS) $(INCLUDE)
	$(CC) $(MAIN_TARGETS) $(LIBS) -o $(MAIN_OUT)

## QuadTree class
$(QUADTREE_OUT): $(QUADTREE_REQS) $(INCLUDE)
	$(CC) -c $(QUADTREE_TARGETS)

## Object class
$(OBJECT_OUT): $(OBJECT_REQS) $(INCLUDE)
	$(CC) -c $(OBJECT_TARGETS)

clean:
	rm -f *~ *.o *.out
