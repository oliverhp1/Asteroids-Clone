#include <stdio.h>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Asteroid.h"
#include "Ship.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

extern const int MAX_N_ASTEROIDS;
extern int N_ASTEROIDS;
extern int A_Counter;
extern Asteroid Asteroids[];

extern Ship gShip;

extern SDL_Renderer* gRenderer;
extern SDL_Window* gWindow;
