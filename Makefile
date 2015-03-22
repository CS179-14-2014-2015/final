#--Source code--
OBJ = final.cpp

#--Compiler used--
CC = g++

#--Libraries we're linking against.--
LIBRARY_LINKS =-lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -std=c++11

#--Name of our exectuable--
OBJ_NAME = final

#--This is the target that compiles our executable--
all : $(OBJS)  
	$(CC) $(OBJ) $(LIBRARY_LINKS) -o $(OBJ_NAME)
