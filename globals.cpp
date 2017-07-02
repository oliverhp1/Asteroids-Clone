#include "globals.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int MAX_N_ASTEROIDS = 5;
int N_ASTEROIDS = 0;
int A_Counter = 0;
Asteroid Asteroids[MAX_N_ASTEROIDS] = {};

Ship gShip;

SDL_Renderer* gRenderer = NULL;
SDL_Window* gWindow = NULL;

