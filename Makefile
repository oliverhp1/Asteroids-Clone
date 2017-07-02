#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
COMPILER_FLAGS = -c -Wall

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_image

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = game

OBJS = main.o Asteroid.o Ship.o RunGame.o globals.o Bullet.o



all: $(OBJ_NAME)

$(OBJ_NAME): $(OBJS)
	$(CC) $(LINKER_FLAGS) $(OBJS) -o $(OBJ_NAME)

main.o: main.cpp globals.h
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) main.cpp

Asteroid.o: Asteroid.cpp Asteroid.h Bullet.h
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) Asteroid.cpp

Ship.o: Ship.cpp Ship.h
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) Ship.cpp

RunGame.o: RunGame.cpp RunGame.h
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) RunGame.cpp

globals.o: globals.cpp globals.h
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) globals.cpp

Bullet.o: Bullet.cpp Bullet.h globals.h
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) Bullet.cpp

clean: 
	rm -rf *.o hello
