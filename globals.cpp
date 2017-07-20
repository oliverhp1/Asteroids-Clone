#include "globals.h"

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 720;

int MAX_N_ASTEROIDS = 6;
int N_ASTEROIDS = 0;
int AsteroidVelocityScale = 1;
int score = 0;
int numExplosions = 8;
int numAsteroidExplosions = 4;

std::vector<Asteroid> Asteroids;
std::vector<Asteroid> ExtraAsteroids;

int AsteroidWidth0 = 0;
int AsteroidHeight0 = 0;
int BulletWidth = 0;
int BulletHeight = 0;
int ExplosionWidth = 0;
int ExplosionHeight = 0;
int AsteroidExplosionWidth = 0;
int AsteroidExplosionHeight = 0;
int InfernoWidth = 0;
int InfernoHeight = 0;

SDL_Texture* AsteroidTexture = NULL;
SDL_Texture* BulletTexture = NULL;
SDL_Texture* Background = NULL;
SDL_Texture* InfernoBackground = NULL;
SDL_Texture* ExplosionSpriteSheet = NULL;		
SDL_Texture* AsteroidExplosionSpriteSheet = NULL;		

SDL_Rect ExplosionClips[8] = {};
SDL_Rect AsteroidExplosionClips[4] = {};

std::vector<SDL_Rect> rockExplosions[4] = {};
std::vector<SDL_Rect> tempExplosionQueue = {};

Ship gShip;

std::vector<Bullet> Fired;

SDL_Renderer* gRenderer = NULL;
SDL_Window* gWindow = NULL;

SDL_Texture* TextTextures[17] = {NULL}; 
SDL_Rect TextTexture_R[17] = {};

TTF_Font* laserFontB = NULL;
TTF_Font* bloodyFontB = NULL;
TTF_Font* laserFont = NULL;
TTF_Font* bloodyFont = NULL;
TTF_Font* laserFontS = NULL;
TTF_Font* bloodyFontS = NULL;

Mix_Music* menuMusic = NULL;
Mix_Music* playMusic = NULL;
Mix_Music* infernoMusic = NULL;
Mix_Music* deathMusic = NULL;
Mix_Chunk* shoot = NULL;
Mix_Chunk* shot = NULL;
Mix_Chunk* asteroidExplode = NULL;

SDL_Color mainColor = {192,192,192};	// silver
SDL_Color mainColorH = {123,104,238};	// medium slate blue
SDL_Color deathColor = {139,0,0};		// dark red
