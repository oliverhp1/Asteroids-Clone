#OBJS specifies which files to compile as part of project
OBJS = Asteroid.cpp 

#CC specifies the compiler
CC = g++

#COMPILER_FLAGS specifies additional compilation options
#-w suppresses all warnings
COMPILER_FLAGS = -Wall

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_image

#OBJ_NAME specifies the name of our executable
OBJ_NAME = Asteroid

#This is the target that compiles our executable
all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

