#include "globals.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int MAX_N_ASTEROIDS = 5;
int N_ASTEROIDS = 0;
int A_Counter = 0;
std::vector<Asteroid> Asteroids;

int AsteroidWidth = 0;
int AsteroidHeight = 0;
int BulletWidth = 0;
int BulletHeight = 0;

SDL_Texture* AsteroidTexture = NULL;
SDL_Texture* BulletTexture = NULL;

Ship gShip;

std::vector<Bullet> Fired;

SDL_Renderer* gRenderer = NULL;
SDL_Window* gWindow = NULL;

