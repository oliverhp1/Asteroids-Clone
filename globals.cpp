#include "globals.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int MAX_N_ASTEROIDS = 10;
int N_ASTEROIDS = 0;
int score = 0;
std::vector<Asteroid> Asteroids;

int AsteroidWidth0 = 0;
int AsteroidHeight0 = 0;
int BulletWidth = 0;
int BulletHeight = 0;

SDL_Texture* AsteroidTexture = NULL;
SDL_Texture* BulletTexture = NULL;
SDL_Texture* Background = NULL;

Ship gShip;

std::vector<Bullet> Fired;

SDL_Renderer* gRenderer = NULL;
SDL_Window* gWindow = NULL;

SDL_Texture* TextTextures[13] = {NULL}; 
SDL_Rect TextTexture_R[13] = {};
//enum TextTexture_I {Main_Asteroids, Main_Play, Main_Play_H, Main_Instruct, Main_Instruct_H, Main_Quit, Main_Quit_H, Death_Dead, Death_Score, Death_Play, Death_Play_H, Death_Quit, Death_Quit_H};

TTF_Font* laserFontB = NULL;
TTF_Font* bloodyFontB = NULL;
TTF_Font* laserFont = NULL;
TTF_Font* bloodyFont = NULL;

SDL_Color mainColor = {192,192,192};	// silver
SDL_Color mainColorH = {123,104,238};	// medium slate blue
SDL_Color deathColor = {139,0,0};		// dark red
